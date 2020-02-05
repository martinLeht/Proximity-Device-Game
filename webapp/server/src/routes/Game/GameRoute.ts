import express, { Request, Response } from 'express';
import { gameController } from '../../controllers';

export const router = express.Router({
    strict: true
});

router.post('/', async (req: Request, res: Response) => {
    const result: any = await gameController.create(req, res);
    if (result['error']) throw Error(result['error']);
    else {
        res.json(result);
    }
});

router.get('/', async (req: Request, res: Response) => {
    const result:any = await gameController.read(req, res);
    if (result['error']) throw Error(result['error']);
    else {
        res.json(result);
    }
});

router.get('/:id', async (req: Request, res: Response) => {
    const result: any = await gameController.findById(req, res);
    if (result['error']) throw Error(result['error']);
    else {
        res.json(result);
    }
});

router.patch('/:id', async (req: Request, res: Response) => {
    const result: any = await gameController.update(req, res);
    if (result['error']) throw Error(result['error']);
    else {
        res.json(result);
    }
});

router.delete('/:id', async (req: Request, res: Response) => {
    const result: any = await gameController.delete(req, res);
    if (result['error']) throw Error(result['error']);
    else {
        res.json(result);
    }
});