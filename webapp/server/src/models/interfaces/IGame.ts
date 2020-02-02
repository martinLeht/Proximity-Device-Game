export interface IGame extends Document {
    gameId: number;
    greenName: string;
    redName: string;
    greenScore: [];
    redScore: [];
    createdAt: Date;
    updatedAt: Date;
}