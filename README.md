# cloud-drive

## Introduction
This is a cloud drive system designed following the Client-server model. It consists of two main section: friend section and file section.

### Friend Section
In this section, users can add friends, send a new private message or broadcast message.
![Screenshot](/screenshot-1.png)

### File Section
In this section, users can check the files they stored prevously, they can also download the files to their local computer or upload a new file to the cloud drive.
Another feature is that users can share the files they stored with each other.
![Screenshot](/screenshot-2.png)
![Screenshot](/screenshot-3.png)
## Implementation
The implementation involves TCP sockets, multithreading and SQLite3 database. TCP sockets and a HTTP-like protocol allow clients and the server to transfer data efficiently and safely. The multithreading techniques 
enables the server to handle more than one client request at a time, which tremendensly increase the server's efficiency. A SQLite3 database is also built to help store and maintain user information.
