import { Request, Response } from 'express';
import { CrudController } from '../CrudController';

export class GameController implements CrudController {

    public create(req: Request, res: Response): void {
        throw new Error("Method not implemented.");
    }    
    
    public read(req: Request, res: Response): void {
        res.json({ message: 'GET /user request received' });
    }

    public update(req: Request, res: Response): void {
        throw new Error("Method not implemented.");
    }

    public delete(req: Request, res: Response): void {
        throw new Error("Method not implemented.");
    }

    
}