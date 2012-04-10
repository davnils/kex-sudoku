disp('Now importing datafiles');
backtrack
rulebased
disp('Imported datafiles succesfully');
disp('Histograms of the distributions of time are as follows');
bt = [];
index = 1;
unsolved = 0;
unstable = 0;

for i = 1:length(backtracktimes)
    if(backtracktimes(i)>=0)
        bt(index) = backtracktimes(i);
        index = index + 1;
    elseif(backtracktimes(i)==-1)
        unstable = unstable + 1; 
    else%(bakctracktimes(i)==-2)
        unsolved = unsolved + 1;
    end
end

fprintf('Unsolved %d\nUnstable %d\nSolved %d\nTotal %d\n',unsolved,unstable,length(bt),length(backtracktimes));
        
    

hist(bt,500);
axis([0 2 -Inf Inf]); 
%pause;
%hold on;
%hist(rulebasedtimes,500);
%set(gca,'XTick',0:0.1:2);
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
