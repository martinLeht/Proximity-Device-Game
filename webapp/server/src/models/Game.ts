import mongoose, { Schema } from 'mongoose';
import { IGame} from './interfaces/IGame';
const AutoIncrement = require('mongoose-sequence')(mongoose);


// Mongo ODM to mapp to database
const GameSchema: Schema = new Schema({
    gameId: { type: Number},
    greenName: { type: String, default: 'Green', required: true },
    redName: { type: String, default: 'Red', required: true },
    greenScore: [Number],
    redScore: [Number],
    createdAt: { type: Date, default: Date.now },
    updatedAt: { type: Date }
});

// Enables autoincrementation to gameId field
GameSchema.plugin(AutoIncrement, { id: 'game_id_seq', inc_field: 'gameId' });

export default mongoose.model<IGame>('Game', GameSchema);