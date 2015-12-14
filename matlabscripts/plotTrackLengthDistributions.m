%% Plot Path Distance Distribution

nbins = 100;

hf = figure('Name','NF Path Length Distribution in pixels');

subplot(3,1,1);
ConditionIndex = 1;
ResSet                               = vertcat(ExpTrackResults{:,VialPairsPerCondition(ConditionIndex )});
meanConditionLength{ConditionIndex}  = vertcat(ResSet.Length);
[cnt,bin] = hist(meanConditionLength{ConditionIndex},nbins);
hist(meanConditionLength{ConditionIndex},nbins)
title(strcat('OR NF \mu:',num2str(mean(meanConditionLength{ConditionIndex}))));
%xlabel('px distance');
h = findobj(gca,'Type','patch');
set(h,'FaceColor','b');
set(h,'EdgeColor','w');
xlimits = 400;
ylimits =  2*ceil(max(cnt)/10)*10;
ylim([0 ylimits]);
xlim([0 xlimits]);

subplot(3,1,2);
ConditionIndex = 2;
ResSet                               = vertcat(ExpTrackResults{:,VialPairsPerCondition(ConditionIndex )});
meanConditionLength{ConditionIndex}  = vertcat(ResSet.Length);
hist(meanConditionLength{ConditionIndex},nbins);
title(strcat('CT NF \mu:',num2str(mean(meanConditionLength{ConditionIndex}))));
%xlabel('px distance');
ylim([0 ylimits]);
xlim([0 xlimits]);

subplot(3,1,3);
ConditionIndex = 3;
ResSet                               = vertcat(ExpTrackResults{:,VialPairsPerCondition(ConditionIndex )});
meanConditionLength{ConditionIndex}  = vertcat(ResSet.Length);
hist(meanConditionLength{ConditionIndex},nbins);
title(strcat('AB NF \mu:',num2str(mean(meanConditionLength{ConditionIndex}))));
%xlabel('px distance');
ylim([0 ylimits]);
xlim([0 xlimits]);
saveas(hf,'figures/NFTrackLengthHist.pdf')

%%
hf = figure('Name','DMSO 0.5% Path Length Distribution ');
subplot(3,1,1);
ConditionIndex = 4;
ResSet                               = vertcat(ExpTrackResults{:,VialPairsPerCondition(ConditionIndex )});
meanConditionLength{ConditionIndex}  = vertcat(ResSet.Length);
hist(meanConditionLength{ConditionIndex},nbins);
title(strcat('OR DMSO 0.5% \mu:',num2str(mean(meanConditionLength{ConditionIndex}))));
h = findobj(gca,'Type','patch');
set(h,'FaceColor','b');
set(h,'EdgeColor','w');
ylim([0 ylimits]);
xlim([0 xlimits]);

subplot(3,1,2);
ConditionIndex = 5;
ResSet                               = vertcat(ExpTrackResults{:,VialPairsPerCondition(ConditionIndex )});
meanConditionLength{ConditionIndex}  = vertcat(ResSet.Length);
hist(meanConditionLength{ConditionIndex},nbins);
title(strcat('CT DMSO 0.5% \mu:',num2str(mean(meanConditionLength{ConditionIndex}))));
ylim([0 ylimits]);
xlim([0 xlimits]);

subplot(3,1,3);
ConditionIndex = 6;
ResSet                               = vertcat(ExpTrackResults{:,VialPairsPerCondition(ConditionIndex )});
meanConditionLength{ConditionIndex}  = vertcat(ResSet.Length);
hist(meanConditionLength{ConditionIndex},nbins);
title(strcat('AB DMSO 0.5% \mu:',num2str(mean(meanConditionLength{ConditionIndex}))));
ylim([0 ylimits]);
xlim([0 xlimits]);
saveas(hf,'figures/DMSO05TrackLengthHist.pdf')

%%
hf = figure('Name','DMSO 1% Path Length Distribution ');
subplot(3,1,1);
ConditionIndex = 7;
ResSet                               = vertcat(ExpTrackResults{:,VialPairsPerCondition(ConditionIndex )});
meanConditionLength{ConditionIndex}  = vertcat(ResSet.Length);
hist(meanConditionLength{ConditionIndex},nbins);
title(strcat('OR DMSO 1% \mu:',num2str(mean(meanConditionLength{ConditionIndex}))));
h = findobj(gca,'Type','patch');
set(h,'FaceColor','b');
set(h,'EdgeColor','w');
ylim([0 ylimits]);
xlim([0 xlimits]);

subplot(3,1,2);
ConditionIndex = 8;
ResSet                               = vertcat(ExpTrackResults{:,VialPairsPerCondition(ConditionIndex )});
meanConditionLength{ConditionIndex}  = vertcat(ResSet.Length);
hist(meanConditionLength{ConditionIndex},nbins);
title(strcat('CT DMSO 1% \mu:',num2str(mean(meanConditionLength{ConditionIndex}))));
ylim([0 ylimits]);
xlim([0 xlimits]);

subplot(3,1,3);
ConditionIndex = 9;
ResSet                               = vertcat(ExpTrackResults{:,VialPairsPerCondition(ConditionIndex )});
meanConditionLength{ConditionIndex}  = vertcat(ResSet.Length);
hist(meanConditionLength{ConditionIndex},nbins);
title(strcat('AB DMSO 1% \mu:',num2str(mean(meanConditionLength{ConditionIndex}))));
ylim([0 ylimits]);
xlim([0 xlimits]);

saveas(hf,'figures/DMSO10TrackLengthHist.pdf')

