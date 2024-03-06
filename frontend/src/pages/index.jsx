import React, { Component } from "react";
import Container from "../components/Container";
import styles from "../styles/Common.module.css";
import Landingstyles from "../styles/Landing.module.css";

export default class LandingPage extends Component {
  render() {
    return (
      <Container main>
        <div className={styles.container}>
          <div className={Landingstyles.wrapper}>
            <h1 className={Landingstyles.title}>
              Dota 2 Analyzer
            </h1>
            <div className={Landingstyles.description}>
              Providing you with <b>statistics and analytics</b> of Dota 2 Matches.
            </div>
            <a 
              href="https://www.kaggle.com/devinanzelmo/dota-2-matches"
              className={`${styles.button} ${Landingstyles.button}`}
            >
              Original Dataset Used
            </a>
          </div>
        </div>
      </Container>
    );
  }
};
