# Personal Database in C!

A project that is almost a clone of SQLite, but wayyyy smaller. This project was built from scratch in C, with a full-stack web-interface using React (Inspired by [SQLite in c](https://cstack.github.io/db_tutorial/)) 

# Features:- 

- SQL-like commands such as 'insert' and 'select'.
- SQL-like metacommands such as '.btree', '.constants', 'exit'
- HTTP server to handle requests and responses, communication facilitated by shared fiels
- Cross-platform compatibility (Linux, macOS, Windows)

# Tech Stack:- 
### Backend (Made solely with C)

- Custom memory-paged database
- B-Tree structure with leaf-nodes
- Sockets for HTTP server requests and responses
- File-based intercommunication between frontend and backend
- Backend supports POSIX and Windows with the help of preprocessors

### Frontend:-

- Frontend Bash like interface in React
- User inputs are sent to the backend using API calls
- Display logs in real-time

# Project prerequisites

- C compiler (Project built with GCC)
- Node.js and npm

# How to Start the Project?

### For backend:-

- In the root of the project, open the terminal and type:-

```
cd backend
gcc main.c src/*.c -IHeaders -o exec/mydb && ./exec/mydb exec/mydb.db
```

### For frontend:-

- In the root of the project, open the terminal and type:-

```
cd frontend
npm install
npm run dev
```

# Usage Example

## To insert a row:-

```
insert 1 John doe@gmail.com
insert 2 Jane doe@gmail.com
```

## View all rows:-

```
select
```

## View tree structure:-

```
.constant
.btree
```

## Exit and clean-up:-

```
.exit
```

# Communication Flow

- React UI captures the user input via forms and sends a `POST` request to backend.
- Backend receives this input as plain text through the server.
- Server writes this command in `CommsFiles/command.txt` file.
- The program reads this command and executes it.
- The result is then logged into the `CommsFiles/output.txt` file.
- The server reads the logs from the `CommsFiles/output.txt` file and sends it to the frontend as plain text

# License

MIT License.