import { IGame } from './IGame';
import { FormGroup, Validators, FormControl, FormArray } from '@angular/forms';


export class Game implements IGame{
    gameId: number;    
    greenName: string;
    redName: string;
    goal: number;
    greenScore: number[];
    redScore: number[];
    createdAt: string;

    static asFormGroup(game: IGame): FormGroup {
        const fg = new FormGroup({
            gameId: new FormControl(game.gameId, Validators.required),
            greenName: new FormControl(game.greenName, Validators.required),
            redName: new FormControl(game.redName, Validators.required),
            goal: new FormControl(game.goal, Validators.required),
            greenScore: new FormArray([new FormControl(game.greenScore)]),
            redScore: new FormArray([new FormControl(game.redScore)]),
            createdAt: new FormControl(game.createdAt, Validators.required)
        });
        return fg;
    }

}