import random
import sys

def main():
    total_data_node = int(sys.argv[1])
    random_null_data_node = random.randint(1,total_data_node)
    print(total_data_node)
    print(random_null_data_node)
    comp_list = []
    for i in range(total_data_node):
        temp_list = None
        prob = random.random()
        if(random_null_data_node>0 and  prob<0.5):
            print('creating empty list')
            temp_list = []
            random_null_data_node-=1
        elif(i+random_null_data_node>=total_data_node):
            print('creating empty list')
            temp_list = []
            random_null_data_node-=1
        else:
            print('creating full list')
            temp_list = create_random_list()
        
        comp_list.append(temp_list)
    print_for_database(comp_list)
    
#takes 2d list
# prints lists, if the list is empty prints null
def print_for_database(comp_list):
    for x in comp_list:
        if(len(x)>0):
            print(x)
        else:
            print('NULL')
def create_random_list():
    random_list = []
    random_len = random.randint(1,10)
    print('random_len in create_random_list',random_len)
    for i in range(random_len):
        random_list.append(random.randint(0,10))
    return random_list

if __name__=='__main__':
    main()

