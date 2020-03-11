import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders, HttpErrorResponse } from '@angular/common/http';
import { Observable, throwError } from 'rxjs';
import { map } from 'rxjs/operators';
import { IGame } from '../models/IGame';
import { FormArray } from '@angular/forms';
import { Game } from '../models/Game';

@Injectable({
  providedIn: 'root'
})
export class GamesService {

  readonly API_URL: string = 'https://proximity-game-server.herokuapp.com/games/'; //'http://localhost:3000/games/';
  private httpOptions = {
    headers: new HttpHeaders({
      'Content-Type':  'application/json'
    })
  };

  constructor(private http: HttpClient) {}

  getGames(): Observable<IGame[]> {
    return this.http.get<IGame[]>(this.API_URL);
  }

  getAll(): Observable<IGame[]> {
    return this.http.get<IGame[]>(this.API_URL)
    .pipe(map((games: IGame[]) => {
      return games.slice(8, 12);
    }));
  }

  getGamesAsFormArray(): Observable<FormArray> {
    return this.getGames().pipe(map((games: IGame[]) => {
      const fgs = games.map(Game.asFormGroup);
      return new FormArray(fgs);
    }));
  }

  update(id: number, game: IGame): Observable<any> {
    return this.http.patch(this.API_URL + id, game, this.httpOptions);
  }

  delete(id: number): Observable<any> {
    return this.http.delete(this.API_URL + id, this.httpOptions);
  }

  // Handle Errors 
  error(error: HttpErrorResponse) {
    let errorMessage = '';
    if (error.error instanceof ErrorEvent) {
      errorMessage = error.error.message;
    } else {
      errorMessage = `Error Code: ${error.status}\nMessage: ${error.message}`;
    }
    console.log(errorMessage);
    return throwError(errorMessage);
  }

}
