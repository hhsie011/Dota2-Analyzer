import React, { Component } from "react";
import { BrowserRouter as Router, Route } from "react-router-dom";
import { Chart } from "react-chartjs-2";

import LandingPage from "./pages/index";
import HeroesPage from "./pages/heroes";
import AbilitiesPage from "./pages/abilities";
import PlayersPage from "./pages/players";
import MatchesPage from "./pages/matches";

import "./styles/globals.css";

Chart.defaults.global.animation.duration = 800;

class App extends Component {
  render() {
    return (
      <Router>
        <Route exact path="/" component={LandingPage} />
        <Route exact path="/heroes" component={HeroesPage} />
        <Route exact path="/abilities" component={AbilitiesPage} />
        <Route exact path="/players" component={PlayersPage} />
        <Route exact path="/matches" component={MatchesPage} />
      </Router>
    );
  }
}

export default App;