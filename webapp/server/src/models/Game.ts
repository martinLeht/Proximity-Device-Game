import mongoose, { Schema } from 'mongoose';

const GameSchema: Schema = new Schema({
    gameId: { type: Number, required: true },
    greenName: { type: String, default: 'Green', required: true },
    redName: { type: String, default: 'Red', required: true },
    greenScore: [{
        type: Number
     }],
    redScore: [{ 
        type: String
    }],
    createdAt: { type: Date, default: Date.now },
    updatedAt: { type: Date }
}, );