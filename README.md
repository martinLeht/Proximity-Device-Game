# Proximity Game / Device firmware + Webapp
A school project, where we create a small device to play measuring short distance proximity to a generated value within 3 meters. Game needs 2 players and has a web client where players may view scores. 


## Getting Started
Here are the instructions to get the development environment up and running.

### Prerequisites

* [Node](https://nodejs.org/)
* [Angular](https://angular.io/)
* [Docker] (https://www.docker.com/products/docker-desktop) IF you wish to run application in cloud
* [Mongo] (https://www.mongodb.com/cloud/atlas): Either a local mongodatabase or a remote cluster in cloud.
(If database is local, you will not be able to save game results from the device)

### Want to deploy the server in a docker container?
Here is a good 3 part article that i followed to deploy the application to heroku:
https://wwachira.hashnode.dev/how-to-create-a-node-app-within-a-docker-container-cjvzdbl7h001lc3s1k6sofdi3


### Initial setup

Clone the repository and install the dependencies on both projects

```bash
git clone https://github.com/martinLeht/Proximity-Device-Game.git
```

```bash
cd webapp/client
npm install
```

```bash
cd webapp/server
npm install
```

### Configure docker files, if you wish to deploy it to the cloud. If you just want a local application, skip to > "Running Node Server"
Files: Docker, docker-compose.yml

* Docker:
```docker
# Define the image we will use and version
# latest just means we need the latest nodejs image available
FROM node:8

# Create an app directory to hold the application code
WORKDIR /marleh/Embedded-systems/proximity-game/webapp/server/src

# Duplicate the dependency file to the container's project root directory.
COPY package*.json ./

# Install app dependencies
RUN npm install

# Bundle app source inside the docker image
COPY . .

# Expose our app port inside the app and 
EXPOSE 3000:3000

# Define commands that will run the app
CMD ["npm", "run", "dev"]
```	  
		  
* docker-compose.yml:
```yml
# Defines our composer file version
version: "2.2"
# Define our services 
services: 
  # This is our nodejs app built using the Dockerfile
  app:
	# The name of our node app container
	container_name: proximity_game_server
	# Restarts our node app whenever it fails
	restart: always
	# Builds the node app docker container from the local -
	# Docker file we created
	build: .
	# Exposes the necessary ports that our node app uses
	ports:
	  - "3000:3000"
	# All the services our node app will link to -
	# which in our case is only mongo
	# You can other services such as a Redis
	links:
	  # The name should be similar to the service you will build
	  - mongo
  # Our database service called mongo
  mongo:
	# The name of the database container, NOTE: it is similar to the name provided
	container_name: mongo
	# Builds a mongo image from the docker repository
	image: mongo
	# Exposes the ports that Mongo uses
	ports:
	  - "27017:27017"
```


### Running Node Server
Server configuration:
* create .env in root of server (webapp/server) and modify according your environment

```env
PORT=3003 // port where the application is running
MONGO_URI="mongodb+srv://<mongoUsername>:<password>@<collection>-86xni.mongodb.net/test?retryWrites=true&w=majority" // uri to mongo cluster in atlas (there is a section in atlas to get it)
MONGO_USER="XXX" // Atlas mongo username
MONGO_PASSWORD="XXX" // Atlas mongo password
```

```bash
cd webapp/server
npm run dev
```

### Running Angular Frontend
Configure URL that the service uses to make API requests:
* Open webapp/client/src/app/services/games.service.ts in the editor and modify the root URL
```angular
readonly API_URL: string = 'http://localhost:3003/games/'; // path to the API running on server
```

```bash
cd webapp/client
ng serve
```

### Running Server side API unit tests

```bash
cd webapp/server
npm run test-dev
```


### Building Angular Frontend for production

```bash
cd webapp/client
ng build --prot
```
Built files can be found from `client/dist/hoods-login`


