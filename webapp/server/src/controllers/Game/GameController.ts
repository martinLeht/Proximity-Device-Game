import { Request, Response } from 'express';
import { CrudController } from '../CrudController';
import Game from './../../models/Game';
import { IGame } from '../../models/interfaces/IGame';


export class GameController implements CrudController {

    // Creates a game entry with provided scores.
    public async create(req: Request, res: Response): Promise<any> {
        
        return new Promise<any>(async (resolve, reject) => {
            const goal: number = req.body.goal;
            const greenScore: number[] = req.body.greenScore;
            const redScore: number[] = req.body.redScore;

            const game: IGame = new Game({
                goal: goal,
                greenScore: greenScore,
                redScore: redScore
            });

            console.log("Green Scores: " + req.body.greenScore);
            console.log("Red Scores: " + req.body.redScore);
            
            try {
                const savedResult = await game.save();
                const parsedResult = this.parse(savedResult);
                resolve(parsedResult);
            } catch(err) {
                reject({
                    error: err
                });
            }
        });
        
    }    
    
    // Fetches all the games from mongoDb
    public async read(req: Request, res: Response): Promise<any> {
        return new Promise<any>(async (resolve, reject) => {
            try {
                const result: any[] = await Game.find({});
                const parsedResult: any[] = result.map(entry => this.parse(entry));
                resolve(parsedResult);
            } catch (err) {
                reject({
                    error: err
                });
            }
        });
    }

    // Fetches a game entry by id
    public async findById(req: Request, res: Response): Promise<any> {
        return new Promise<any>(async (resolve, reject) => {
            const id: number = parseInt(req.params.id);
            if (!id) reject({ error: 'No id provided' });
            try {
                const result: any = await Game.findOne({ gameId: id });
                const parsedResult: any = this.parse(result);
                resolve(parsedResult);
            } catch (err) {
                reject({
                    error: err
                });
            }
        });
    }

    // Updates a game by given id
    public async update(req: Request, res: Response): Promise<any> {
        return new Promise<any>(async (resolve, reject)=> {
            const id: number = parseInt(req.params.id);
            if (!id) reject({ error: 'Procide a id' });

            let gameData: any = {
                greenName: req.body.greenName,
                redName: req.body.redName
            };

            if (!gameData.greenName && !gameData.redName) 
                reject({ error: 'No data provided for updating' });
            
            gameData = this.clean(gameData);
            console.log("Data to be updated: " + gameData);

            try {
                await Game.findOneAndUpdate({ gameId: id }, gameData);
                console.log('Successfully updated a game entry with id: ' + id);
                resolve({
                    success: 'Successfully updated a game entry with id: ' + id
                });
            } catch(err) {
                reject({
                    error: err
                });
            }
        });
    }

    // Deletes a game with given id
    public async delete(req: Request, res: Response): Promise<any> {
        return new Promise<any>(async (resolve, reject) => {
            const id: number = parseInt(req.params.id);
            if (!id) reject({ error: 'Procide a id' });

            try {
                await Game.findOneAndDelete({ gameId: id });
                console.log('Successfully deleted a game entry with id: ' + id);
                resolve({
                    success: 'Successfully deleted a game entry with id: ' + id
                });
            } catch(err) {
                reject({
                    error: err
                });
            }
        });
    }

    // Parses the entry with right names
    private parse(entry: any) {
        return {
            gameId: entry["gameId"],
            greenName: entry["greenName"],
            redName: entry["redName"],
            goal: entry["goal"],
            greenScore: entry["greenScore"],
            redScore: entry["redScore"],
            createdAt: entry["createdAt"],
            updatedAt: entry["updatedAt"]
        };
    }

    // Cleans out the field that are empty
    private clean(data: any): any {
        for (let prop in data) { 
            if (data[prop] === null || data[prop] === undefined) {
                delete data[prop];
            }
        }
        return data;
    }
    
}