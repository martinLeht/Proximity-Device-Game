import express,{ Request, Response } from 'express';
import helmet from 'helmet';
import cors from 'cors';
import http, { Server } from 'http';
import mongoose, { Connection } from 'mongoose';

import { gameRouter } from './routes';
import { PORT, MONGO_URI } from './config/config';


// Base application class or "Front Controller"
export class App {

    private app: express.Application;
    private server: Server;

    constructor() {
        this.app = express();
        this.initMiddlewares();
        this.initRoutes();
        
        this.server = http.createServer(this.app);
        this.server.on('listening', () => {
            try {
                const mongoUri: string = MONGO_URI as string;
                this.establishMongoConnection(mongoUri);
            } catch (err) {
                console.error(`Unable to connect to Mongo!`, err);
            }
        });
    }

    // Initialises middleware
    private initMiddlewares(): void {
        this.app.use(cors());
        this.app.use(helmet());
        this.app.use(express.json());
        this.app.use(express.urlencoded());
    }

    // Initiates the routes
    private initRoutes(): void {
        this.app.use('/games', gameRouter);
    }

    // Creates a connection to the mongo database
    private establishMongoConnection(url: string): void {
        mongoose.connect(url, {
            useNewUrlParser: true,
            useFindAndModify: false,
            useUnifiedTopology: true
        }).then(() => {
            return console.info(`Successfully connected to ${url}`);
        }).catch(error => {
            console.error('Error connecting to database: ', error);
            return process.exit(1);
        });
    }

    // Starts listening on the server
    public serverListen(): void {

        this.server.listen(PORT, () => {
            console.log(`Server listening at port: ${PORT}`);
        });
    }
}

export default App;