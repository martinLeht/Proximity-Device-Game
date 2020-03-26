import { Document } from 'mongoose';

// game interface used by the ODM
export interface IGame extends Document {
    gameId: number;
    greenName: string;
    redName: string;
    greenScore: number[];
    redScore: number[];
    createdAt: Date;
    updatedAt: Date;
}