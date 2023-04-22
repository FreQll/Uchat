# <p align=center>Uchat | Abyss</p>

This is our implementation of the real-time messaging app for the Ucode project. 

<p align=center><a href="http://www.youtube.com/watch?feature=player_embedded&v=Geh1b6XWR6E" target="_blank"><img src="http://img.youtube.com/vi/Geh1b6XWR6E/0.jpg" 
alt="Abyss" width="320" height="240" border="10" /></a></p>

The chat was created by:

***[oshcherbat](https://github.com/BigTako)*** - Server-side Developer,<br>
***[mcherednyc](https://github.com/Nastroyki)*** - Server-side Developer,<br>
***[amorozenko](https://github.com/xqudee)*** - Client-side Developer,<br>
***[dshchepin](https://github.com/FreQll)*** - Team lead | Client-side Developer.<br>

<h1 align=center> Installation </h1>

**Important!** You need to already have installed GTK library on your device, so if you do not - install it first.

```brew install gtk+3```

1. Download/clone the repo and move to the directory.
2. To *build* the project type ```make``` in the terminal.
3. To run the server, type ```./uchat_server PORT``` where *PORT* is some number starting from 1030.
4. Now, you can run the client part which will be connected to the mentioned server and i recommend you to create a new terminal window for this. Type ```./uchat IPv4 PORT``` where *IPv4* is your address in the local network (if you are going to use client part on the same machine that server, type 127.0.0.1) and *PORT* is a number that you entered in the previous step.

To delete project enter ```make uninstall``` and to rebuild the project - ```make reinstall```.

<h1 align=center> Implemented features </h1>

* **Avatar change** Each user can change the default profile picture from out of 30 prepared in advance. In the future it is planned to make it possible to insert your own photo, as well as to generate an avatar depending on the initials of the nickname. <br> <p align="center"><img src="https://user-images.githubusercontent.com/62791316/233790074-c5423696-ab4a-4887-ad1f-9ba5ee423ef1.png" alt="GTK+" width="350" height="400"></p>

* **Choosing a theme** In settings, user can change collors of the application (including login and signup windows). Now available in the following colors: 
1. Standard dark 
2. White
3. Ocean blue
4. Yellow
5. Pink 
6. Purple <br><br><p align="center"><img src="https://user-images.githubusercontent.com/62791316/233790305-5247153a-7758-4975-8634-d3aa74af38c8.png" alt="Screenshot1" width="400" height="300"><img src="https://user-images.githubusercontent.com/62791316/233790312-ca0712a6-96a8-449b-9efa-0afab1053c4b.png" alt="Screenshot2" width="400" height="300"><img src="https://user-images.githubusercontent.com/62791316/233790331-f52e4de0-e97d-4bef-8c40-c9d212c6b00a.png" alt="Screenshot3" width="400" height="300"></p> <br>

* **Editing, deleting, copying messages** User can delete, edit, copy their own message, and only copy other messages. Also user can delete whole chat. And the coolest part is that delete and edit messages working in real time! <br> ![image](https://user-images.githubusercontent.com/62791316/233791304-aabeef2e-6b90-4dc1-b4e3-57268dbf3ec4.png)<br>

* **Reconnection** If the app lost connection with the server, client will constantly try to reconnect. Appropriate message and gif will be displayed. <br> <p align="center">![image](https://user-images.githubusercontent.com/62791316/233791650-e7cac0a8-2e7e-48ed-acbf-91b705346824.png)</p><br>

* ```uchat_server``` works as a **daemon**, listening to the specified port. 

* All necessary data is saved into the **database**. SQLite3 database was used.

* Messages and data encryption using OpenSSH

<h1 align=center> Used tools </h1>

<p align="center"><img src="https://blog.desdelinux.net/wp-content/uploads/2018/09/gtk-logo.png" alt="GTK+" width="400" height="200"><img src="https://upload.wikimedia.org/wikipedia/commons/thumb/5/59/Glade_3_logo.svg/114px-Glade_3_logo.svg.png" alt="Glade" width="251" height="198"><img src="https://upload.wikimedia.org/wikipedia/commons/thumb/d/d5/CSS3_logo_and_wordmark.svg/1200px-CSS3_logo_and_wordmark.svg.png" alt="CSS" width="200" height="250"><img src="https://upload.wikimedia.org/wikipedia/commons/thumb/3/38/SQLite370.svg/1200px-SQLite370.svg.png" alt="SQLite" width="400" height="200"><img src="https://upload.wikimedia.org/wikipedia/commons/thumb/6/6a/OpenSSL_logo.svg/278px-OpenSSL_logo.svg.png" alt="OpenSSL" width="250" height="100"></p>
