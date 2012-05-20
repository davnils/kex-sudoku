% Fixa bort alla unsolved och unstable
%bmachine
boltzmann_fast_decline
%bmachine_medium
boltzmann_medium_decline_raw

bt = [];
index = 1;
unsolved = 0;
unstable = 0;

for i = 1:length(bmachine)
    if(bmachine(i)>=0)
        bt(index) = bmachine(i);
        index = index + 1;
    elseif(bmachine(i)==-1)
        unstable = unstable + 1; 
    else%(bakctracktimes(i)==-2)
        unsolved = unsolved + 1;
    end
end

fprintf('Unsolved %d\nUnstable %d\nSolved %d\nTotal %d\n',unsolved,unstable,length(bt),length(bmachine));
% För att jämföra med exponentialfördelning
xexp = 0:0.001:20;
[fbt xbt] = ksdensity(bt,xexp,'function','pdf'); 
plot(xbt,fbt,'r-');
axis([0 20 -Inf Inf]);
hold on;


bt = [];
index = 1;
unsolved = 0;
unstable = 0;

for i = 1:length(bmachine_medium)
    if(bmachine_medium(i)>=0)
        bt(index) = bmachine_medium(i);
        index = index + 1;
    elseif(bmachine_medium(i)==-1)
        unstable = unstable + 1; 
    else%(bakctracktimes(i)==-2)
        unsolved = unsolved + 1;
    end
end

fprintf('Unsolved %d\nUnstable %d\nSolved %d\nTotal %d\n',unsolved,unstable,length(bt),length(bmachine_medium));
% För att jämföra med exponentialfördelning
xexp = 0:0.001:20;
[fbt xbt] = ksdensity(bt,xexp,'function','pdf'); 
plot(xbt,fbt,'b-');
axis([0 20 -Inf Inf]);
hold on;


