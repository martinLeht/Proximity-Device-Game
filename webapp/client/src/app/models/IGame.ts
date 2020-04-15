export interface IGame {
    gameId: number;
    greenName: string;
    redName: string;
    goal: number;
    greenScore: number[];
    redScore: number[];
    createdAt: string;
}