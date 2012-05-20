% Fixa bort alla unsolved och unstable
backtrack
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
% För att jämföra med exponentialfördelning
xexp = 0:0.001:20;
fexp = exppdf(xexp,mean(bt));
plot(xexp,fexp,'b-');
hold on;
[fbt xbt] = ksdensity(bt,xexp,'function','pdf'); 
plot(xbt,fbt,'r-');
axis([0 10 -Inf Inf]);
pause;
hold off;

%För att plotta sorterat index mot tid
plot(1:length(bt),sort(bt));
pause;

%För att plotta histogram
[Y X] = hist(bt,40);
plot(X,Y,'.k');

%pause;
%axis([0 2 0 Inf]);
%pause;
%axis([2 20 0 800]);
%pause;




