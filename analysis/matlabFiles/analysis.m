disp('Now importing datafiles');
backtrack
rulebased
disp('Imported datafiles succesfully');
disp('Histograms of the distributions of time are as follows');
%hist(backtracktimes,500);
%pause;
%hold on;
hist(rulebasedtimes,500);
set(gca,'XTick',0:0.05:2);
% Create xlabel
%xlabel({'Solution times (seconds)'});

% Create ylabel
%ylabel({'Occurences in testing of 49151 puzzles'});

%pause;
%hold off;
%disp('The probability functions look like this');

%ksdensity(backtracktimes);
%pause;
%hold on;
%ksdensity(rulebasedtimes);
%pause;
