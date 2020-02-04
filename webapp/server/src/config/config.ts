import * as dotenv from "dotenv";

dotenv.config();
let path = `${__dirname}/../../../.env`;
dotenv.config({ path: path });

export const PORT = process.env.PORT;
export const MONGO_URI = process.env.MONGO_URI;
