# DOCKET SETUP
```bash
dcbuild
dcup
```
open a new tab of terminal
```bash
dockps
#copy and edit in hosts
sudo gedit /etc/hosts
```

to start with a clean database
```bash
sudo rm -rf mysql_data
```

# LAB TASKS
## TASK 1

```bash
sudo gedit /etc/hosts &>/dev/null &
```

Go to seed-server

<a href="www.seed-server.com"> Seed Server</a>


open html header live add on

username : alice <br>
password : seedalice <br>


you can see username and pass on html header live
<br><br>

## TASK 2

visit samy website
<a href="www.attacker32.com"> Attacker32 </a>

Go to add friend option<br>

See the page source. we need to modify img src <br>

log out alice<br>
log in samy<br>

start http header live and add alice as a friend

log out samy. and log in alice.

in cmd 
```bash
ls
cd attacker/
#open a shell in atacker
#open a new tab of cmd
docksh aac0eeed82a9
```
now
```bash
cd /var/www/attacker/
nano addfriend.html
```

## TASK 3
log out ALICE
<br> log in SAMY
<br> EDIT PROFILE
<br>open HTML Header Live
<br> Write "Samy is my hero." in brief description
<br>now click save btn


### Now in Editprofile.html

Edit **** to relavant values and save

now copy the editprofile.html

```bash
docker cp editprofile.html aac0eeed82a9:/var/www/attacker/
```

check in the shell
```bash
cat editprofile.html
```

### NOW LOGOUT SAMY
### LOG IN ALICE

http://www.attacker32.com/

go to edit profile

## TASK 4

to turn on countermeasure
```bash
#open a new tab of cmd
#elgg
docksh 0f180270060c

cd /var/www/elgg/vendor/elgg/elgg/engine/classes/Elgg/Security
```

remove the
return statement from Csrf.php(validate function)

### now delete friend and samy is my hero description. 
### try to attack now. you will fail.




















