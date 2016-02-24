# DataBase

This database gives an ability to add data from console or from file. Also it provides client - service connection. While writing this DB I was concentrated not on user interface but on speed of the processing.

Here I used red-black trees represented in C++ STL as map. I written my own hashset to use it over one of my indices for better speed.

This DB suppports sql-like interface with specially written syntax analizer. Some of aviable commands you can find here:
`client1/commands.txt`
