import random
import mysql.connector



def execute_queries():
    con = mysql.connector.connect(
        user="admin",
        password="myadminpassword",
        host="127.0.0.1",
        database="aggregator"
    )
    cursor = con.cursor()
    for i in range(125, 150):

        cor1 = []
        cor2 = []
        heat1 = []
        heat2 = []

        for j in range(random.randint(5, 10)):
            cor1.append(random.randint(1, 10))
        for j in range(random.randint(5, 10)):
            cor2.append(random.randint(1, 10))
        for j in range(random.randint(5, 10)):
            heat1.append(random.randint(1, 40))
        for j in range(random.randint(5, 10)):
            heat2.append(random.randint(1, 40))

        torque = []
        for j in range(random.randint(5, 10)):
            torque.append(random.randint(100, 400))

        price = []
        for j in range(random.randint(5, 10)):
            price.append(random.randint(50_000, 520_000))
        
        fuel = []
        for j in range(random.randint(5, 10)):
            fuel.append(random.randint(4, 20))

        acce = []
        for j in range(random.randint(5, 10)):
            acce.append(random.randint(3, 15))
        
        topspeed = []
        for j in range(random.randint(5, 10)):
            topspeed.append(random.randint(120, 300))

        brake = []
        for j in range(random.randint(5, 10)):
            brake.append(random.randint(40, 200))

        
        query = """INSERT INTO cars VALUES ({},
            '{}',
            '{}',
            '{}',
            '{}',
            '{}',
            '{}',
            '{}',
            '{}',
            '{}',
            '{}' );""".format(str(i), cor1, cor2, heat1, heat2, torque, price, fuel, acce, topspeed, brake)
        print(query)
        break
        cursor.execute(query)
        print("queried " + str(i))
    con.commit()
    cursor.close()
    con.close()


execute_queries()