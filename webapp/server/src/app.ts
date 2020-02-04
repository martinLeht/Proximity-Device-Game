import express,{ Request, Response } from 'express';
import helmet from 'helmet';
import cors from 'cors';
import http, { Server } from 'http';
import mongoose, { Connection } from 'mongoose';

import { gameRouter } from './routes';
import { PORT, MONGO_URI } from './config/config';

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

    private initMiddlewares() {
        this.app.use(cors());
        this.app.use(helmet());
        this.app.use(express.json());
        this.app.use(express.urlencoded());
    }

    private initRoutes() {
        this.app.use('/games', gameRouter);
    }

    private establishMongoConnection(url: string) {
        mongoose.connect(url, {
            useNewUrlParser: true
        }).then(() => {
            return console.info(`Successfully connected to ${url}`);
        }).catch(error => {
            console.error('Error connecting to database: ', error);
            return process.exit(1);
        });
    }

    public serverListen() {

        this.server.listen(PORT, () => {
            console.log(`Server listening at port: ${PORT}`);
        });
    }
}

export default App;