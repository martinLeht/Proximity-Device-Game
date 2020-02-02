import express,{ Request, Response } from 'express';
import helmet from 'helmet';
import cors from 'cors';
import http from 'http';

import { gameRouter } from './routes';

import { PORT } from './config/config';

export class App {

    private app: express.Application;

    constructor() {
        this.app = express();
        this.initMiddlewares();
        this.initRoutes();
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

    public serverListen() {
        const server = http.createServer(this.app);

        server.listen(PORT, () => {
            console.log(`Server listening at port: ${PORT}`);
        });
    }
}

export default App;