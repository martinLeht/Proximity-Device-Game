import 'mocha';
import { expect, assert } from 'chai';
import { App } from '../src/app';
import chai from 'chai';
import chaiHttp from 'chai-http';

import * as dotenv from "dotenv";

let path = `${__dirname}/../../.env`;
dotenv.config({ path: path });

const port = process.env.PORT;
const app = new App();
app.serverListen();

chai.use(chaiHttp);
describe("Functionality of Games REST API", () => {
  it("Gets game results from API", done => {
    chai
      .request(`localhost:${port}`)
      .get("/games")
      .end((err, res) => {
        expect(res).to.have.status(200);
        done();
      });
  });
  let gameIdOnCreation: number;
  it("Inserts game entry through API", done => {
    chai
      .request(`localhost:${port}`)
      .post("/games")
      .send({
          goal: 123,
          greenScore: [1, 2, 3],
          redScore: [4, 5, 6]
      })
      .end((err, res) => {
        const gameEntry = res.body;
        gameIdOnCreation = gameEntry.gameId;
        expect(res).to.have.status(200);
        expect(gameEntry.greenName).to.equals("Green");
        expect(gameEntry.redName).to.equals("Red");
        expect(gameEntry.goal).to.equals(123);

        assert.isArray(gameEntry.greenScore);
        assert.isArray(gameEntry.redScore);
        const greenScore: Array<number> = gameEntry.greenScore;
        const redScore: Array<number> = gameEntry.redScore;

        let expectedGreenPoint = 1;
        let expectedRedPoint = 4;
        for (let i=0; i < 3; i++) {
            expect(greenScore[i]).to.equals(expectedGreenPoint);
            expect(redScore[i]).to.equals(expectedRedPoint);
            expectedGreenPoint++;
            expectedRedPoint++;
        }
        done();
      });
  });

  it("Gets game result by game id from API", done => {
    chai
      .request(`localhost:${port}`)
      .get("/games/" + gameIdOnCreation)
      .end((err, res) => {
        expect(res).to.have.status(200);
        const gameEntry = res.body;
        expect(res).to.have.status(200);
        expect(gameEntry.gameId).to.equals(gameIdOnCreation);
        expect(gameEntry.greenName).to.equals("Green");
        expect(gameEntry.redName).to.equals("Red");
        expect(gameEntry.goal).to.equals(123);

        assert.isArray(gameEntry.greenScore);
        assert.isArray(gameEntry.redScore);
        const greenScore: Array<number> = gameEntry.greenScore;
        const redScore: Array<number> = gameEntry.redScore;

        let expectedGreenPoint = 1;
        let expectedRedPoint = 4;
        for (let i=0; i < 3; i++) {
            expect(greenScore[i]).to.equals(expectedGreenPoint);
            expect(redScore[i]).to.equals(expectedRedPoint);
            expectedGreenPoint++;
            expectedRedPoint++;
        }
        done();
      });
  });

  it("Updates player names from a game result by game id from API", done => {
    chai
      .request(`localhost:${port}`)
      .patch("/games/" + gameIdOnCreation)
      .send({
        greenName: "Frodo",
        redName: "Sam"
      })
      .end((err, res) => {
        expect(res).to.have.status(200);
        expect(res.body.success).to.equals('Successfully updated a game entry with id: ' + gameIdOnCreation);
        done();
      });
  });

  it("Gets game result by game id from API to ensure update worked", done => {
    chai
      .request(`localhost:${port}`)
      .get("/games/" + gameIdOnCreation)
      .end((err, res) => {
        expect(res).to.have.status(200);
        const gameEntry = res.body;
        expect(res).to.have.status(200);
        expect(gameEntry.gameId).to.equals(gameIdOnCreation);
        expect(gameEntry.greenName).to.equals("Frodo");
        expect(gameEntry.redName).to.equals("Sam");
        expect(gameEntry.goal).to.equals(123);

        assert.isArray(gameEntry.greenScore);
        assert.isArray(gameEntry.redScore);
        const greenScore: Array<number> = gameEntry.greenScore;
        const redScore: Array<number> = gameEntry.redScore;

        let expectedGreenPoint = 1;
        let expectedRedPoint = 4;
        for (let i=0; i < 3; i++) {
            expect(greenScore[i]).to.equals(expectedGreenPoint);
            expect(redScore[i]).to.equals(expectedRedPoint);
            expectedGreenPoint++;
            expectedRedPoint++;
        }
        done();
      });
  });

  it("Deletes game entry through API", done => {
    chai
      .request(`localhost:${port}`)
      .delete("/games/" + gameIdOnCreation)
      .end((err, res) => {
        expect(res).to.have.status(200);
        expect(res.body.success).to.equals('Successfully deleted a game entry with id: ' + gameIdOnCreation);
        done();
      });
  });
});