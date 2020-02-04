import mongoose, { Schema } from 'mongoose';

//const AutoIncrement = require('mongoose-sequence')(mongoose);

const GameSchema: Schema = new Schema({
    gameId: { type: Number, required: true },
    greenName: { type: String, default: 'Green', required: true },
    redName: { type: String, default: 'Red', required: true },
    greenScore: [{
        type: Number
     }],
    redScore: [{ 
        type: Number
    }],
    createdAt: { type: Date, default: Date.now },
    updatedAt: { type: Date }
});

export default mongoose.model('Game', GameSchema);