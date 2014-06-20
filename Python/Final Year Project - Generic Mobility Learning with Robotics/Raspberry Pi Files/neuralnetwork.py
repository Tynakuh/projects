from pybrain.structure import LinearLayer, SigmoidLayer,TanhLayer, BiasUnit
from pybrain.structure import FeedForwardNetwork
from pybrain.structure import FullConnection
from pybrain.tools.customxml.networkwriter import NetworkWriter
from pybrain.tools.customxml.networkreader import NetworkReader
import string
import numpy as np
import random
import servo as rb
import time

MUTATE_CHANCE = 1.00
GEN = 0
POP_SIZE = 50 
STEPS = 20
LAST_REWARD = 100.0
totalRewards = np.array([])

def createRandomNetwork():
    n = FeedForwardNetwork()
    inLayer = LinearLayer(8)
    hiddenLayer = TanhLayer(6)
    outLayer = TanhLayer(2)
    bias = BiasUnit('bias')
    n.addInputModule(inLayer)
    n.addModule(hiddenLayer)
    n.addOutputModule(outLayer)
    n.addModule(bias)
    in_to_hidden = FullConnection(inLayer,hiddenLayer)
    hidden_to_out = FullConnection(hiddenLayer,outLayer)
    bias_to_hidden = FullConnection(bias, hiddenLayer)
    bias_to_out = FullConnection(bias, outLayer)
    n.addConnection(in_to_hidden)
    n.addConnection(hidden_to_out)
    n.addConnection(bias_to_hidden)
    n.addConnection(bias_to_out) 
    n.sortModules()
    rand_gen_seq = np.random.uniform(-1,1,96)
    n._setParameters(rand_gen_seq)
    return n

def generateRandomNetworks(size):
    netList = np.ndarray(shape=(size,2),dtype = object) #netList[n][0] = Network, netList[n][1] = Reward 
    count = 0
    while (count < (netList.size/netList[0].size)):
        netList[count][0]  = createRandomNetwork()
        netList[count][1] = float(0)
        count = count + 1
    return netList


def crossover(parent):
    parents = parent.copy()
    cross_prob = 0.0+parents[0][1]/(0.0+parents[0][1]+parents[1][1])
    p1genome = parents[0][0].params
    p2genome = parents[1][0].params
    for x in range(0,p1genome.size): #for each gene in the chromosone crossover they have a % probability (based upon reward) to exchange genes to create a child
        if (random.random()<=cross_prob):
            temp = p1genome[x] 
            p1genome[x] = p2genome[x]
            p2genome[x] = temp
    parents[0][0]._setParameters(p1genome)
    parents[1][0]._setParameters(p2genome)
    return parents
    
def generateChildren(parent):
    networks = np.ndarray(shape=(POP_SIZE,2),dtype = 'object')
    for i in range(0,POP_SIZE): #Modify to take second child aswell if desired.
        networks[i][0] = crossover(parent)[0][0].copy()
        networks[i][1] = float(0)
    return networks

def mutate(netList): #Randomly mutate networks by mutate chance
    for count in range(10,POP_SIZE): #Start from 10, don't mutate elites.
        if (random.random() <= MUTATE_CHANCE):
            print count
            temp = netList[count][0].params
            mutated = random.randrange(0,temp.size-1)
            temp[mutated] = random.uniform(-1,1)
            netList[count][0]._setParameters(temp)
    return netList
    

def evaluate(network):
    totalReward = 100.0
    global LAST_REWARD
    LAST_REWARD = 100.0
    for i in range(0,STEPS):
        if(i == STEPS-1): #reset to prone allowing for 'patterns'
            rb.setAngles([20.0,20.0])
            time.sleep(0.3)
            totalReward = totalReward + getRewards(rb.getSensors())
            return totalReward
        totalReward = totalReward + step(network)+network.params[0]
	print totalReward
    return totalReward

def step(network):
    actions = network.activate(rb.getSensors())
    actions[0] = (-actions[0] + 1)/2 *40
    actions[1] = (actions[1] + 1)/2 *40
    rb.setAngles(actions)
    time.sleep(0.2)
    rewards = getRewards(rb.getSensors())
    return rewards

def pickAndGen(netList):
    newList = np.ndarray(shape=(POP_SIZE,2),dtype = 'object')
    for count2 in  range(0,10,1): #Keep top 10, elitism.
        newList[count2][0] = netList[count2][0].copy()
        print newList[count2][0].params
        newList[count2][1] = 0.0
    print "NEXT"
    for count in  range(11,POP_SIZE,2):
        parents = np.ndarray(shape=(2,2),dtype = 'object')
        parent1 = pickParent(netList).copy()
        parent2 = pickParent(netList).copy()
        parents[0] = parent1
        parents[1] = parent2
        temp = crossover(parents).copy()
        parents[0][0] = temp[0][0]
        print parents[0][0]
        parents[0][1] = 0.0
        parents[1][0] = temp[1][0]
        parents[1][1] = 0.0
        newList[count-1] = parents[0]
        newList[count] = parents[1]
    return newList

def pickParent(netList):
    rand = random.random()
    for count in range(0,POP_SIZE):
        value = netList[count][1]
        if(rand<value):
            return netList[count]
        else:
            rand = rand - value
            
def getRewards(array):
    global LAST_REWARD
    LAST_REWARD = LAST_REWARD + array[1]
    return LAST_REWARD

##def startLearn(netList):
##    for count in range(0,POP_SIZE):
##        netList[count][1] = evaluate(netList[count][0]) #Evaluate each neural network for their reward
##    netList = (netList[netList [:,1].argsort()])[::-1] #Sort by second column in descending order
##    saveNetworks(netList[:2].copy())
##    print netList[:5]
##    netList = generateChildren(netList[:2]) #take top two
##    netList = mutate(netList)
##    saveNetworks(netList)
##    return netList

def startLearn(netList):
    global totalRewards
    loopTotal = 0.0
    for count in range(0,POP_SIZE):
        reward = evaluate(netList[count][0]) #Evaluate each neural network for their reward
        netList[count][1] = reward
        loopTotal += reward
    netList = (netList[netList[:,1].argsort()])[::-1] #Sort by second column in descending order
    saveNetworks(netList)
    for count in range(0,POP_SIZE):
        netList[count][1] = netList[count][1]/loopTotal
    #totalRewards = np.append(totalRewards,netList[0][1])
    #saveNetworks(netList[:2].copy())
    netList = pickAndGen(netList)
    #netList = generateChildren(netList[:2]) #take top two
    netList = mutate(netList)
    return netList


def main():
    netList = generateRandomNetworks(POP_SIZE)
    count = 0
    while 1:
        netList = startLearn(netList)
    rb.stop()

def saveNetworks(netList):
    global GEN
    global MUTATE_CHANCE
    f = open('/home/pi/network' +str(GEN)+ '.txt','w')
    for count in range(0,POP_SIZE):
        f.write(str(netList[count][0].params) + str(netList[count][1]))
    #f.write(str(netList[0][0].params)+'\n'+str(netList[1][0].params)+'\n' + str(totalRewards))
    f.close()
    GEN= GEN+1
    MUTATE_CHANCE = MUTATE_CHANCE - 0.01 #annealing
    return

main()
