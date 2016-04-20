%Produces plot of Mean Speed / Activity based on Number of tracklet samples

%Conditions Label Are in  process FileTracks
nbins = 100;
ylimits = 7;
ylimitsTracklets =  2500;
clear meanConditionSpeeds;
clear mu;
clear n;
clear stdd;

%cd /media/kostasl/FlashDrive/PilotVialTrack/ExpSet2_201603/DataOut %Home
%load(strcat('LarvaTrackData',strOutputTag,'.mat'));
%% Do Mean Speed Per Condition Per Time Window
meanConditionSpeeds  = {};
n  = zeros(length(ExpTrackResultsInTime),9);
mu  = zeros(length(ExpTrackResultsInTime),9);
stdd  = zeros(length(ExpTrackResultsInTime),9);
ConditionIndex = 1;


for t=1:length(ExpTrackResultsInTime)
    for (ConditionIndex=1:ConditionIndexMax)
            ExpTrackResults         = ExpTrackResultsInTime{t};
            %TODO: Add Filters Here
            %ExpTrackResults =             
            
            ResSet                  = vertcat(ExpTrackResults{:,VialPairsPerCondition(ConditionIndex,: )});

            if isempty(ResSet)
                continue;
            end
            meanConditionSpeeds{ConditionIndex}   = vertcat(ResSet.MeanSpeed);
            n(t,ConditionIndex)                   = length(meanConditionSpeeds{ConditionIndex});
            mu(t,ConditionIndex)                  = mean(meanConditionSpeeds{ConditionIndex});
            stdd(t,ConditionIndex)                = std(meanConditionSpeeds{ConditionIndex});
    end
    
end

ylimitsTracklets = ceil(max(n(:))/100)*100;
ylimits = ceil(max(mu(:)));

strtitle = sprintf('Mean Activity -Sliding Window %d hours',TimeFrameWidth/3600);
% Plot Results - In Groups or - In sets of 3-genotypes For each Food Condition
CondGrouping = ConditionIndexMax; %When =ConditionIndexMax then plot them all together

for i=1:length(ConditionGroups) %plot Per Condition Groups 
    CondIndexes = ConditionGroups{i};
    
%    for (ConditionIndex=1:CondGrouping:ConditionIndexMax)

        t = length(ExpTrackResultsInTime);
        Exptime = (VialAge(1)+(1:t)*timeAdvance)/3600;
        hf = figure('Name',strcat(ExpCondFood{CondIndexes(1)},strtitle));

        subplot(3,1,1)
        %plot(Exptime,mu(:,ConditionIndex:(ConditionIndex+CondGrouping-1))   );
        plot(Exptime,mu(:,CondIndexes)   );
        title('Mean Speed in px/sec');
        ylim([0 ylimits]);
        subplot(3,1,2)
        plot(Exptime,n(:,CondIndexes));
        title('Number of samples');
        hh= subplot(3,1,3)
        %get(hh,'position')

        plot(Exptime,stdd(:,CondIndexes));
        title('STD Dev ');
        xlabel('Hour');
        ylim([0 ylimits]);

        %Make Legend
        strLegend = '';
        j = 0;
        for k=1:length(CondIndexes)
            j=j+1; 
            strLegend{j} = strcat(ExpCondFood{CondIndexes(k)},ExpCondTitles{CondIndexes(k)});
        end
        legend( strLegend,'Location','southoutside','Orientation','vertical','Position',[0.84 0.01 0.124 0.43])
        %set(hh,'position',[0.13 0.2 0.77 0.12]); %Fix Last plot after adding legends
        saveas(hf,strcat('figures/meanVial',strOutputTag,'SpeedSlidingWindow',ExpCondFood{CondIndexes(1)},'-G',num2str(i),'.png'));
%    end
end %end Condition Groups
%,strcat(ExpCondFood{ConditionIndex+1},ExpCondTitles{ConditionIndex+1}),strcat(ExpCondFood{ConditionIndex+2},ExpCondTitles{ConditionIndex+2})

%% PLOT ALL GENOTYPES %%%
for (ConditionIndex=1:CondGrouping:ConditionIndexMax)
        CondIndexes = ConditionIndex:(ConditionIndex+CondGrouping-1);
        t = length(ExpTrackResultsInTime);
        Exptime = (VialAge(1)+(1:t)*timeAdvance)/3600;
        hf = figure('Name',strcat(ExpCondFood{ConditionIndex},strtitle));

        subplot(3,1,1)
        plot(Exptime,mu(:,ConditionIndex:(ConditionIndex+CondGrouping-1))   );
        title('Mean Speed in px/sec');
        ylim([0 ylimits]);
        subplot(3,1,2)
        plot(Exptime,n(:,CondIndexes));
        title('Number of samples');
        hh= subplot(3,1,3)
        %get(hh,'position')

        plot(Exptime,stdd(:,CondIndexes));
        title('STD Dev ');
        xlabel('Hour');
        ylim([0 ylimits]);

        %Make Legend
        strLegend = '';
        j = 0;
        for k=1:length(CondIndexes)
            j=j+1; 
            strLegend{j} = strcat(ExpCondFood{CondIndexes(k)},ExpCondTitles{CondIndexes(k)});
        end
        legend( strLegend,'Location','southoutside','Orientation','vertical','Position',[0.84 0.01 0.124 0.43])
        %set(hh,'position',[0.13 0.2 0.77 0.12]); %Fix Last plot after adding legends
        saveas(hf,strcat('figures/meanALLVial',strOutputTag,'SpeedSlidingWindow',ExpCondFood{CondIndexes(1)},'.png'));
%    end
end %end Condition Groups

%% Plot Histogram Of Speed Within A chosen time Window
goToHour = 110;
t= round((goToHour*3600 - VialAge(1))/timeAdvance);
ExpTrackResults = ExpTrackResultsInTime{t};

strtitle = sprintf('Speed Histogram -@t:%d for %d hours',goToHour,TimeFrameWidth/3600);

for i=1:length(ConditionGroups) %plot Per Condition Groups 
    CondIndexes = ConditionGroups{i};

    hf = figure('Name',strcat(ExpCondFood{CondIndexes(1)},strtitle));
    for (k=1:length(CondIndexes))
        ConditionIndex = CondIndexes(k);
        t = length(ExpTrackResultsInTime);
        Exptime = (VialAge(1)+(1:t)*timeAdvance)/3600;

        subplot(length(CondIndexes),1,k);
        ResSet                               = vertcat(ExpTrackResults{:,VialPairsPerCondition(ConditionIndex,: )});
        meanConditionSpeeds{ConditionIndex}  = vertcat(ResSet.MeanSpeed);
        [cnt,bin]                            = hist(meanConditionSpeeds{ConditionIndex},nbins);
        hist(meanConditionSpeeds{ConditionIndex},nbins);
        n       = length(meanConditionSpeeds{ConditionIndex});
        mu      = mean(meanConditionSpeeds{ConditionIndex});
        stdd    = std(meanConditionSpeeds{ConditionIndex});
        strTitle = sprintf('%s %s mean: %0.3f std:%0.3f n:%d',ExpCondFood{ConditionIndex},ExpCondTitles{ConditionIndex},mu,stdd, n );
        title(strTitle);

        %Make Legend
        strLegend = '';
        j = 0;
        for k=1:length(CondIndexes)
            j=j+1; 
            strLegend{j} = strcat(ExpCondFood{CondIndexes(k)},ExpCondTitles{CondIndexes(k)});
        end
        %legend( strLegend,'Location','southoutside','Orientation','vertical','Position',[0.84 0.01 0.124 0.43])
        %set(hh,'position',[0.13 0.2 0.77 0.12]); %Fix Last plot after adding legends
        saveas(hf,strcat('figures/SpeedHistTracklet',strOutputTag,'tHour',num2str(goToHour),'_',ExpCondFood{CondIndexes(1)},'-G',num2str(i),'.png'));
    end
end %end Condition Groups




%% Plot Histogram Of Speed Within A chosen time Window
t= round((goToHour*3600 - VialAge(1))/timeAdvance);
ExpTrackResults = ExpTrackResultsInTime{t};

hold off;
ConditionIndex = 1;


hf = figure('Name',strcat(ExpCondFood{ConditionIndex},'MEAN SPEED'));

subplot(3,1,1);
ResSet                               = vertcat(ExpTrackResults{:,VialPairsPerCondition(ConditionIndex,: )});
meanConditionSpeeds{ConditionIndex}  = vertcat(ResSet.MeanSpeed);
[cnt,bin]                            = hist(meanConditionSpeeds{ConditionIndex},nbins);
hist(meanConditionSpeeds{ConditionIndex},nbins);
n       = length(meanConditionSpeeds{ConditionIndex});
mu      = mean(meanConditionSpeeds{ConditionIndex});
stdd    = std(meanConditionSpeeds{ConditionIndex});
strTitle = sprintf('%s %s mean: %0.3f std:%0.3f n:%d',ExpCondFood{ConditionIndex},ExpCondTitles{ConditionIndex},mu,stdd, n );
title(strTitle);
% h = findobj(gca,'Type','patch');
% set(h,'FaceColor','blue');
% set(h,'EdgeColor','w');

ylimits =  2*ceil(max(cnt)/10)*10;
xlimits = 10;
ylim([0 ylimits]);
xlim([0 xlimits]);

subplot(3,1,2);
ConditionIndex = 2;
ResSet                               = vertcat(ExpTrackResults{:,VialPairsPerCondition(ConditionIndex,: )});
meanConditionSpeeds{ConditionIndex}  = vertcat(ResSet.MeanSpeed);
hist(meanConditionSpeeds{ConditionIndex},nbins);
n       = length(meanConditionSpeeds{ConditionIndex});
mu      = mean(meanConditionSpeeds{ConditionIndex});
stdd    = std(meanConditionSpeeds{ConditionIndex});
strTitle = sprintf('%s %s mean: %0.3f std:%0.3f n:%d',ExpCondFood{ConditionIndex},ExpCondTitles{ConditionIndex},mu,stdd, n );
title(strTitle);
ylim([0 ylimits]);
xlim([0 xlimits]);

subplot(3,1,3);
ConditionIndex = 3;
ResSet                               = vertcat(ExpTrackResults{:,VialPairsPerCondition(ConditionIndex,: )});
meanConditionSpeeds{ConditionIndex}  = vertcat(ResSet.MeanSpeed);
hist(meanConditionSpeeds{ConditionIndex},nbins);
n       = length(meanConditionSpeeds{ConditionIndex});
mu      = mean(meanConditionSpeeds{ConditionIndex});
stdd    = std(meanConditionSpeeds{ConditionIndex});
strTitle = sprintf('%s %s mean: %0.3f std:%0.3f n:%d',ExpCondFood{ConditionIndex},ExpCondTitles{ConditionIndex},mu,stdd, n );
title(strTitle);
ylim([0 ylimits]);
xlim([0 xlimits]);
saveas(hf,sprintf('figures/NFTracklet%sSpeedHist-%dHour.png',strOutputTag,goToHour))


%%Box Plot Of Mean Speeds per tracklet
ConditionIndex = 1;
hf = figure('Name',strcat(ExpCondFood{ConditionIndex},'Crawl-Run SPEEDs'));
groups = [ zeros( length(meanConditionSpeeds{1}) ,1); ones(length(meanConditionSpeeds{2}),1); 2*ones(length(meanConditionSpeeds{3}),1) ];
boxplot([meanConditionSpeeds{1};meanConditionSpeeds{2};meanConditionSpeeds{3}],groups,'labels',{strcat(ExpCondFood{1},ExpCondTitles{1}),strcat(ExpCondFood{2},ExpCondTitles{2}),strcat(ExpCondFood{3},ExpCondTitles{3})})
ylim([0 15]);
saveas(hf,sprintf('figures/NFTracklet%sSpeedBoxPlot-%dHour.png',strOutputTag,goToHour));


%% Do Scatter Plots Of Speeds
hold off;

hf = figure('Name',strcat(ExpCondFood{ConditionIndex},' Tracklet mean speeds across time'));
startT               = round((goToHour*3600 - VialAge(1))/timeAdvance);

for ConditionIndex=1:ConditionIndexMax
    display(ConditionIndex);
    
    cnt = 0;
    speeds = [];
    ttime = [];
    for t=1:length(ExpTrackResultsInTime)
        cnt = cnt+1;
        ExpTrackResults = ExpTrackResultsInTime{t};
        ResSet                               = vertcat(ExpTrackResults{:,VialPairsPerCondition(ConditionIndex,: )});
        if isempty(ResSet) 
            continue;
        end
        meanConditionSpeeds{ConditionIndex}  = vertcat(ResSet.MeanSpeed);
        Exptime = (VialAge(1)+(1:t)*timeAdvance)/3600;

        mxy = max(meanConditionSpeeds{ConditionIndex});
        if ylimits < mxy 
            ylimits = mxy
        end

        n       = length(meanConditionSpeeds{ConditionIndex});
        %mu      = mean(meanConditionSpeeds{ConditionIndex});
        %stdd    = std(meanConditionSpeeds{ConditionIndex});
        %title(strTitle);
        for i=1:length(meanConditionSpeeds{ConditionIndex})
            speeds(cnt) = meanConditionSpeeds{ConditionIndex}(i);
            ttime(cnt)  = (t*timeAdvance + VialAge(1))/3600;
        end

    end

    hold off;
    subplot(ConditionIndexMax,1,ConditionIndex)
    strTitle = sprintf('%s %s ',ExpCondFood{ConditionIndex},ExpCondTitles{ConditionIndex} );
    scatter(ttime,speeds,'.');
    ylim([0 ylimits]);
    xlim([80 (maxRecordingTime + VialAge(1))/3600]);

    title(strTitle);

end
saveas(hf,sprintf('figures/TrackletMeanSpeedScatter-%s-%dHour.png',strOutputTag,goToHour))

