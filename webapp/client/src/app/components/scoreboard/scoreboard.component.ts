import { Component, OnInit, ViewChild, ChangeDetectorRef } from '@angular/core';
import { MatTableDataSource } from '@angular/material/table';
import { MatPaginator } from '@angular/material/paginator';
import { IGame } from 'src/app/models/IGame';
import { GamesService } from 'src/app/services/games.service';
import { FormGroup, FormBuilder, FormArray } from '@angular/forms';

@Component({
  selector: 'app-scoreboard',
  templateUrl: './scoreboard.component.html',
  styleUrls: ['./scoreboard.component.css']
})

export class ScoreboardComponent implements OnInit {

  displayedColumns: string[] = ['gameId', 'playerOne', 'playerTwo', 'playerOneScores', 'playerTwoScores', 'createdAt', 'actions'];

  gameForm: FormGroup;

  gamesData: IGame[] = [];
  formEditOn = false;

  dataSource: MatTableDataSource<IGame>;

  @ViewChild(MatPaginator, {static: true}) paginator: MatPaginator;

  constructor(private gamesService: GamesService,
              private fb: FormBuilder) {}

  ngOnInit() {
    this.gameForm = this.fb.group({
      games: this.fb.array([])
    });

    this.refreshScoreboard();

    /*
    this.dataSource = new MatTableDataSource<IGame>(this.games);
    this.dataSource.paginator = this.paginator;
    this.refreshScoreboard();
    */
  }

  get games(): FormArray {
    return this.gameForm.get('games') as FormArray;
  }

  refreshScoreboard() {
    this.gamesService.getGamesAsFormArray().subscribe(games => {
      games.disable();
      this.gameForm.setControl('games', games);
    });

    this.gamesService.getGames().subscribe(games => {
      this.gamesData = games;
    });
  }

  editToggle(game: FormGroup) {
    if (game.enabled) {
      game.disable();
    } else {
      game.enable();
      game.get('gameId').disable();
      game.get('greenScore').disable();
      game.get('redScore').disable();
      game.get('createdAt').disable();
    }
  }

  remove(id: number) {
    this.gamesService.delete(id).subscribe(res => {
      if (res.success) {
        console.log(res.success);
        this.refreshScoreboard();
      } else {
        console.log(res.error);
      }
      
    });
  }

  save(game: FormGroup) {
    const id: number = game.get('gameId').value;
    this.gamesService.update(id, game.value).subscribe(res => {
      if (res.success) {
        console.log(res.success);
        this.refreshScoreboard();
      } else {
        console.log(res.error);
      }
    }, error => {
      console.log(error);
    });
  }

}
