% Fixa bort alla unsolved och unstable
rulebased
rb = [];
index = 1;
unsolved = 0;
unstable = 0;

for i = 1:length(rulebasedtimes)
    if(rulebasedtimes(i)>=0)
        rb(index) = rulebasedtimes(i);
        index = index + 1;
    elseif(rulebasedtimes(i)==-1)
        unstable = unstable + 1; 
    else%(rulebasedtimes(i)==-2)
        unsolved = unsolved + 1;
    end
end

fprintf('Unsolved %d\nUnstable %d\nSolved %d\nTotal %d\n',unsolved,unstable,length(rb),length(rulebasedtimes));

%För att plotta sorterat index mot tid
plot(1:length(rb),sort(rb));
pause;

%För att plotta histogram
hist(rb,500);
pause;
axis([0 0.1 0 11000]);
pause;
% Oklart om något behövs på resten av histogrammet.
hist(rb,240); 
axis([0.1 1.5 0 60]);
%pause;



