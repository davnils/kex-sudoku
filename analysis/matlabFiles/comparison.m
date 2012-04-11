% Fixa bort alla unsolved och unstable
backtrack
rulebased
cp = [];
index = 1;

%rulebasedtimes och backtracktimes har samma antal tider
for i = 1:min(length(rulebasedtimes),length(backtracktimes))
    if(rulebasedtimes(i)>=0 && backtracktimes(i)>=0)
        cp(index) = backtracktimes(i)-rulebasedtimes(i);
        index = index + 1;
    end
end

%find out when breakpoint occurs
cp = sort(cp);
for i = 1:length(cp)
    if(cp(i)>0)
        fprintf('Backtrackes performs better than rulebased at %d puzzles\n',i-1);
        break;
    end
end

%plot difference
plot(1:length(cp),sort(cp));


%Check if puzzle difficulty correlates

bt = [];
rb = [];

for i = 1:length(backtracktimes)
    if(backtracktimes(i)==-2)
        bt(i)=100;%Ansätt högt värde.
    elseif(backtracktimes(i)==-1)
        bt(i) = median(backtracktimes); % Ansätt värde som inte kommer räknas
    else
        bt(i)=backtracktimes(i);
    end
end

for i = 1:length(rulebasedtimes)
    if(rulebasedtimes(i)==-2)
        rb(i)=100;%Ansätt högt värde.
    elseif(rulebasedtimes(i)==-1)
        rb(i)=median(rulebasedtimes(i)); % Ansätt värde som inte räknas
    else
        rb(i)=rulebasedtimes(i);
    end
end

hardbt = [];
easybt = [];
hardrb = [];
easyrb = [];

tbt = sort(bt);
trb = sort(rb);
bt10 = tbt(floor(length(bt)/10))
rb10 = trb(floor(length(rb)/10))
bt90 = tbt(floor(length(bt)*9/10))
rb90 = trb(floor(length(rb)*9/10))

lowbtindices = [];
for i = 1:length(bt)
    if(bt(i)<=bt10)
        lowbtindices(length(lowbtindices)+1)=i;
    end
end
lowrbindices = [];
for i = 1:length(rb)
    if(rb(i)<=rb10)
        lowrbindices(length(lowrbindices)+1)=i;
    end
end
highbtindices = [];
for i = 1:length(bt)
    if(bt(i)>=bt90)
        highbtindices(length(highbtindices)+1)=i;
    end
end
highrbindices = [];
for i = 1:length(rb)
    if(rb(i)>=rb90)
        highrbindices(length(highrbindices)+1)=i;
    end
end
disp('lengths lowbt, highbt, lowrb, highrb')
length(lowbtindices)
length(highbtindices)
length(lowrbindices)
length(highrbindices)

disp('same low and high')
samelow = length(intersect(lowbtindices,lowrbindices))
samehigh = length(intersect(highbtindices,highrbindices))

disp('samelow/10 % of bt')
samelow/length(lowbtindices)
disp('samelow/10 % of rb')
samelow/length(lowrbindices)
disp('samehigh/10 % of bt')
samehigh/length(highbtindices)
disp('samehigh/10 % of rb')
samehigh/length(highrbindices)


disp('Felmarginaler')
disp('low')
1-binocdf(samelow,length(lowbtindices),length(lowrbindices)/length(rulebasedtimes))
1-binocdf(samelow,length(lowrbindices),length(lowbtindices)/length(backtracktimes))
disp('high')
1-binocdf(samehigh-1,length(highbtindices),length(highrbindices)/length(rulebasedtimes))
1-binocdf(samehigh-1,length(highrbindices),length(highbtindices)/length(backtracktimes))


