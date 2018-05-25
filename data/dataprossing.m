clc, clear, close all;

weights500=[0.951214,0.453641,0.00242738,0.42682,0.353641,0.678034,0.102427,0.226821,0.878034];

weights700=[0.878034,0.578034,0.00242738,0.42682,0.753641,0.553641,0.0780342,0.678034,0.178034];
weights1000=[0.829248,0.504855,0.229248,0.153641,0.304855,0.529248,-0.0195384,0.204855,0.780462];
weights1200=[0.926821,0.526821,0.00242738,0.226821,0.826821,0.702427,0.126821,0.726821,0.226821];

x = [1:9];
Wei = [weights500', weights700', weights1000', weights1200'];
bar(x,Wei)
title('Weights for higest winning rates');

ylabel('Weight')

names = {'EnterBoard','MoveToSafeZone','SendEnemyHome','Block','MoveNormal','Move2Star','Move2Globe','MoveInSafeZone','finishPiece'};
set(gca,'xticklabel',names)

legend("500","700","1000","1200")

%set(gcf, 'Position', [200, 200, 1000, 250])

%%
close all;
gen500_mean = mean(geneWin500)
gen700_mean = mean(geneWin700)
gen1000_mean = mean(geneWing1000)
gen1200_mean = mean(geneWin1200)
x=[1:length(geneWin500)]
x2=[1:length(geneWin700)]
x3=[1:length(geneWing1000)]
x4=[1:length(geneWin1200)]
hold on
plot(x,geneWin500)
plot(x,ones(length(geneWin500),1)*gen500_mean, '-r')

plot(x2,geneWin700)
plot(x2,ones(length(geneWin700),1)*gen700_mean, '-r')

plot(x3,geneWing1000)
plot(x3,ones(length(geneWing1000),1)*gen1000_mean, '-r')

plot(x4,geneWin1200)
plot(x4,ones(length(geneWin1200),1)*gen1200_mean, '-r')
title('Games won over different generation and with their mean');
ylabel('Games won')
xlabel('Generations')
legend("500","700","1000","1200")

%err = geneWin500 - gen500_mean
%figure
%hold on
%plot(x,geneWin500)
%plot(x,ones(length(geneWin500),1)*gen500_mean, '-r')
%x = 1:10:100;
%y = [20 30 45 40 60 65 80 75 95 90]; 
%err = [5 8 2 9 3 3 8 3 9 3];
%errorbar(x,geneWin500,err)


gen500_var = var(geneWin500)
gen700_var = var(geneWin700)
gen1000_var = var(geneWing1000)
gen1200_var = var(geneWin1200)

%% played different pos

p1=[373320,216388,202926,207366];

p2=[214656,373281,212546,199517];
p3=[206916,212941,369111,211032];
p4=[217652,206762,212104,363482];

x = [1:4];
Wei = [p1', p2', p3', p4'];
bar(x,Wei)
title('Wins based on different starting positions');

ylabel('Total wins')

names = {'GA as player 1','GA as player 2','GA as player 3','GA as player 4'};
set(gca,'xticklabel',names)

%legend("500","700","1000","1200")

%% played Q-learning 10k games

x_vs = [1:10]
figure 
hold on
plot(x_vs,Q)
plot(x_vs,GAe3)
plot(x_vs,RandP3)
plot(x_vs,RandP4)
legend("Q-learning","Genetic Algorithm","Random player 3","Random player 4")
title('Q-learning VS GA and randoms players with 10k games');

ylabel('Total wins per trial')
xlable('Trials')