function K = normalizedKernel(K,options)

if options == 1
    for i=1:size(K,1)
        for j=1:size(K,2)
            if i ~= j
                K(i,j) = K(i,j)/min(K(i,i),K(j,j));
            end
        end
    end
    for i=1:size(K,1)
        K(i,i) = 1;
    end

elseif options == 2
    DD = diag(K);    
    for i=1:size(K,1)
        K(i,:) = K(i,:)/sqrt(DD(i));
        K(:,i) = K(:,i)/sqrt(DD(i));
    end
end
