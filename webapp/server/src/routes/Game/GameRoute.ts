import express, { Request, Response } from 'express';
import { gameController } from '../../controllers';

/*
    Here is API that routes to the corresponding controller
    to perform CRUD (Create, Read, Update, Delete) functionalities
*/


export const router = express.Router({
    strict: true
});

// @route POST /game/
// @desc Route to create a game entry
router.post('/', async (req: Request, res: Response) => {
    const result: any = await gameController.create(req, res);
    if (result['error']) throw Error(result['error']);
    else {
        res.json(result);
    }
});

// @route GET /game/
// @desc Route to get all games
router.get('/', async (req: Request, res: Response) => {
    const result:any = await gameController.read(req, res);
    if (result['error']) throw Error(result['error']);
    else {
        res.json(result);
    }
});

// @route GET /game/:id
// @desc Route to get one game by Id
router.get('/:id', async (req: Request, res: Response) => {
    const result: any = await gameController.findById(req, res);
    if (result['error']) throw Error(result['error']);
    else {
        res.json(result);
    }
});

// @route PATCH /game/:id
// @desc Route to update a game by Id
router.patch('/:id', async (req: Request, res: Response) => {
    const result: any = await gameController.update(req, res);
    if (result['error']) throw Error(result['error']);
    else {
        res.json(result);
    }
});

// @route DELETE /game/:id
// @desc Route to delete a game entry by id
router.delete('/:id', async (req: Request, res: Response) => {
    const result: any = await gameController.delete(req, res);
    if (result['error']) throw Error(result['error']);
    else {
        res.json(result);
    }
});