function our_mask = computeSegmentation(numFrames,nr,nc,param,sortInd,frame,BPLR_)

Sc = ones(2)-eye(2); % smoothness cost

INF_ = 5000;
SE1 = strel('disk', 25);
SE2 = strel('disk', 5);

our_mask = zeros(nr,nc,numFrames);
finalizedFrames = zeros(numFrames,1);
lin = reshape(1:nr*nc, [nr nc]);

for i=1:length(sortInd)
    if finalizedFrames(sortInd(i))==1
        continue;
    end

    totNodes = nr*nc*3;
    Dc = zeros(2, totNodes);
    
    count = 0;
    Ndx = [];
    Val = [];
    for j=sortInd(i)-1:sortInd(i)+1
        if ( (j>0) && (j<=numFrames) )
            offset = count*nr*nc;
            
            % data term
            if BPLR_ == 1
                temp = -log(param.alpha_*frame(j).Dc + (1-param.alpha_)*frame(j).BPLR_Dc); 
            else
                temp = -log(frame(j).Dc);
            end
            infNdx = isinf(temp);
            temp(infNdx) = INF_;            
            Dc(:, offset+1:offset+nr*nc) = reshape(temp(:), [nr*nc 2])';               
                  
            % 4-neighborhood intra-image smoothness term
            Ndx = [Ndx; vec(sub2ind([totNodes totNodes], lin(:,1:end-1)+offset, lin(:,2:end)+offset))]; 
            Val = [Val; vec(frame(j).Hc(:,1:end-1))];
            Ndx = [Ndx; vec(sub2ind([totNodes totNodes], lin(1:end-1,:)+offset, lin(2:end,:)+offset))];
            Val = [Val; vec(frame(j).Vc(1:end-1,:))];

            if ( (j~=sortInd(i)+1) && (j<numFrames) )
                % 2-neighborhood inter-image smoothness term
                Ndx = [Ndx; vec(sub2ind([totNodes totNodes], lin+offset, frame(j).opticalflow_map+offset+nr*nc))];
                Val = [Val; vec(frame(j).opticalflow_coloraff)];
            end

            count = count + 1;
        end
    end
    Dc(:, count*nr*nc+1:end) = [];
    
    [Ndx, sortNdx] = sort(Ndx,'ascend');
    Val = Val(sortNdx);
    
    Vc = createSparseAdjMatrixFast(Ndx-1,Val,totNodes,totNodes);
    Vc(:, count*nr*nc+1:end) = [];
    Vc(count*nr*nc+1:end,:) = [];    
    Vc = max(Vc, Vc'); % make it symmetric
    
    %%%%%%%%%%%%%%
    %%% graph cuts        
    gch = GraphCut('open',Dc,Sc,Vc);
    [gch Seg] = GraphCut('expand',gch);
    gch = GraphCut('close',gch);          
    %%%%%%%%%%%%%%
    Seg = double(Seg);
    
    % fix fg/bg labels of frames that are already segmented (with slight
    % erosion/dilation for label propagation
    count = 0;
    for j=sortInd(i)-1:sortInd(i)+1
        if (j>0) && (j<=numFrames) 
            
            if finalizedFrames(j)==0 && j==sortInd(i)
                offset = count*nr*nc;                
                thisSeg = reshape(Seg(offset+1:offset+nr*nc), [nr, nc]);
                our_mask(:,:,j) = thisSeg;
                
                dilateSeg = imdilate(thisSeg, SE1);
                dilateBgInd = find(dilateSeg==0);
                
                erodeSeg = imerode(thisSeg, SE2);
                erodeFgInd = find(erodeSeg==1);
                
                frame(j).Dc(dilateBgInd) = 1;
                frame(j).Dc(dilateBgInd+nr*nc) = 0;
                frame(j).Dc(erodeFgInd) = 0;
                frame(j).Dc(erodeFgInd+nr*nc) = 1;                
                
                if BPLR_ == 1
                    frame(j).BPLR_Dc(dilateBgInd) = 1;
                    frame(j).BPLR_Dc(dilateBgInd+nr*nc) = 0;
                    frame(j).BPLR_Dc(erodeFgInd) = 0;
                    frame(j).BPLR_Dc(erodeFgInd+nr*nc) = 1;              
                end
                
                finalizedFrames(j) = 1;
            end

            count = count + 1;
        end
    end   
end