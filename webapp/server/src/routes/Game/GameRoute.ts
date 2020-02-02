import express, { Request, Response } from 'express';
import { gameController } from '../../controllers';

export const router = express.Router({
    strict: true
});

router.post('/', (req: Request, res: Response) => {
    gameController.create(req, res);
});

router.get('/', (req: Request, res: Response) => {
    gameController.read(req, res);
});

router.patch('/', (req: Request, res: Response) => {
    gameController.update(req, res);
});

router.delete('/', (req: Request, res: Response) => {
    gameController.delete(req, res);
});