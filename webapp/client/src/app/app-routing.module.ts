import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';
import { ScoreboardComponent } from './components/scoreboard/scoreboard.component';


const routes: Routes = [
  { path: 'scoreboard', component: ScoreboardComponent },
  { path: '',
    redirectTo: '/scoreboard',
    pathMatch: 'full'
  }
];

@NgModule({
  imports: [RouterModule.forRoot(routes)],
  exports: [RouterModule]
})
export class AppRoutingModule { }
