function plotClusterRates(d,MAXCLUSTERS,NUMITER,featType)

N = length(featType);

Pmean = zeros(N,MAXCLUSTERS);
for k=1:MAXCLUSTERS
    for j=1:length(featType)
        load([d 'k=' num2str(k) '_' char(featType(j)) '.mat'],'iter');
        
        p = zeros(1,NUMITER);
        for l=1:NUMITER        
            p(l) = iter(l).purity;
        end   
        Pmean(j,k) = mean(p);
    end
end

colors_ = ['br'];
shapes_ = ['vs'];

figure;
set(gca,'FontSize',60,'FontWeight','Bold');
xlabel('# of clusters')
ylabel('Purity');
ylim([0 0.6]);
xlim([0 35]);
title('Discovery');

hold on
for i=1:length(featType)
    plot(1:length(Pmean),Pmean(i,:),[colors_(i) shapes_(i) '-'],'LineWidth',5,'MarkerSize',30);
end
hold off
legend('Object-Graph','App. Only');
