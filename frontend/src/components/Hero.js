import React, { Component } from "react";
import { Line, HorizontalBar, Bar } from "react-chartjs-2";
import axios from 'axios';
import styles from "../styles/Common.module.css";
import Herostyles from "../styles/Heroes.module.css";
import Modalstyles from "../styles/Modal.module.css";

import Modal from "./Modal";
import ModalAlt from "./Modal2";

import { MdBuild, MdDelete } from "react-icons/md";
import { IoStatsChart } from "react-icons/io5";


const api = axios.create({ baseURL: 'http://localhost:8080/'});

const options1 = {
  responsive: true,
  legend: { display: false },
  elements: {
    line: { tension: 0 }
  },
  scales: {
    xAxes: [{
      gridLines: {
        zeroLineColor: '#7c86a1',
        color: '#7c86a12a'
      },
      ticks: {
        fontColor: '#7c86a1',
        fontSize: 13,
        fontStyle: '500'
      },
      scaleLabel: {
        display: true,
        labelString: 'TIME (MIN)',
        fontColor: '#49526b',
        fontSize: 16,
        fontStyle: '600'
      }
    }],
    yAxes: [{
      gridLines: {
        zeroLineColor: '#7c86a1',
        color: '#7c86a12a'
      },
      ticks: {
        beginAtZero: true,
        max: 70,
        fontColor: '#7c86a1',
        fontSize: 13,
        fontStyle: '500'
      },
      scaleLabel: {
        display: true,
        labelString: 'WIN RATE (%)',
        fontColor: '#49526b',
        fontSize: 16,
        fontStyle: '600'
      },
    }],
  },
}

const options2 = {
  responsive: true,
  legend: { display: false },
  scales: {
    xAxes: [{
      gridLines: {
        zeroLineColor: '#7c86a1',
        color: '#7c86a12a'
      },
      ticks: {
        beginAtZero: true,
        fontColor: '#7c86a1',
        fontSize: 13,
        fontStyle: '500'
      },
      scaleLabel: {
        display: true,
        labelString: 'WIN % RATIO',
        fontColor: '#49526b',
        fontSize: 16,
        fontStyle: '600'
      }
    }],
    yAxes: [{
      gridLines: {
        color: '#7c86a12a'
      },
      ticks: {
        fontColor: '#7c86a1',
        fontSize: 13,
        fontStyle: '500'
      },
      scaleLabel: {
        display: true,
        labelString: 'HEROES',
        fontColor: '#49526b',
        fontSize: 16,
        fontStyle: '600'
      }
    }]
  },
}
const options3 = {
  responsive: true,
  legend: { 
    display: false,
  },
  scales: {
    xAxes: [{
      gridLines: {
        zeroLineColor: '#7c86a1',
        color: '#7c86a12a'
      },
      ticks: {
        fontColor: '#7c86a1',
        fontSize: 13,
        fontStyle: '500'
      },
    }],
    yAxes: [{
      gridLines: {        
        zeroLineColor: '#7c86a1',
        color: '#7c86a12a'
      },
      ticks: {
        beginAtZero: true,
        fontColor: '#7c86a1',
        fontSize: 13,
        fontStyle: '500'
      },
      scaleLabel: {
        display: true,
        labelString: 'AVERAGE PER MINUTE',
        fontColor: '#49526b',
        fontSize: 16,
        fontStyle: '600'
      },
    }]
  },
}

export default class Hero extends Component {
  constructor() {
    super();
    this.state = {  
      posts: [], 
      analytics1: [],
      analytics2: [],
      analytics3: [],
      heroID: 0,
      heroName: "", 
      winRate: 0, 
      pickRate: 0,
      updateFormOpen: false,
      deleteFormOpen: false,
      analyticsOpen: false,
      modalAltPage: 1,
      a2Start: 0,
      a2End: 20,
      aPage: 1
    };
    this.rank = 1;
    this.selected = { 
      id: 0,
      heroName: "", 
      winRate: 0, 
      pickRate: 0 
    };
    this.handleChangeHeroName = this.handleChangeHeroName.bind(this);
    this.handleChangeWinRate = this.handleChangeWinRate.bind(this);
    this.handleChangePickRate = this.handleChangePickRate.bind(this);
    this.updateHero = this.updateHero.bind(this);
    this.deleteHero = this.deleteHero.bind(this);
  }

  componentDidUpdate() {
    this.rank = 1;
  }

  componentDidMount() {
    axios.get('./../heroes.json')
      .then(res => {
          console.log(res);
          this.setState({posts: res.data});
      })
      .catch(err => { console.log(err); })
    axios.get('./../winrate_over_time.json')
      .then(res => {
          console.log(res);
          this.setState({analytics1: res.data});
      })
      .catch(err => { console.log(err); })
    axios.get('./../heroes_vs_winrate.json')
      .then(res => {
          console.log(res);
          this.setState({analytics2: res.data});
      })
      .catch(err => { console.log(err); })
    axios.get('./../hero_gold_and_xp.json')
      .then(res => {
          console.log(res);
          this.setState({analytics3: res.data});
      })
      .catch(err => { console.log(err); })
  }

  handleChangeHeroName(e) {
    this.setState({ heroName: e.target.value });
    this.selected.heroName = e.target.value;
  }
  handleChangeWinRate(e) {
    this.setState({ winRate: e.target.value });
    this.selected.winRate = e.target.value;
  }
  handleChangePickRate(e) {
    this.setState({ pickRate: e.target.value });
    this.selected.pickRate = e.target.value;
  }

  updateHero() {
    const { id, winRate, pickRate } = this.selected;
    const req = JSON.stringify({
      "request_type": "update_hero",
      "hero_id": id,
      "win_rate": winRate,
      "pick_rate": pickRate
    });
    api.post('/update/hero', req, 
      {headers: {"content-type": "application/json"}})
      .then(res => { console.log(res) })
      .catch(err => { console.log(err) })
  }

  deleteHero() {
    const { id } = this.selected;
    const req = JSON.stringify({
      "request_type": "delete_hero",
      "hero_id": id
    });
    api.post('/delete/hero', req, 
      {headers: {"content-type": "application/json"}})
      .then(res => { console.log(res) })
      .catch(err => { console.log(err) })
  }

  openUpdate = (post) => {
    this.setState({ updateFormOpen: !this.state.updateFormOpen });
    this.selected.id = post.id;
    this.selected.heroName = post.name;
    this.selected.winRate = post.win_rate;
    this.selected.pickRate = post.pick_rate;
  }

  openDelete = (post) => {
    this.setState({ deleteFormOpen: !this.state.deleteFormOpen });
    this.selected.id = post.id;
    this.selected.heroName = post.name;
  }

  openAnalytics = (post) => {
    this.setState({ modalAltPage: 1 });
    this.setState({ a2Start: 0 });
    this.setState({ a2End: 10 });
    this.setState({ aPage: 1 });
    this.setState({ analyticsOpen: !this.state.analyticsOpen });
    this.selected.id = post.id;
    this.selected.heroName = post.name;
    this.selected.pickRate = post.pick_rate;
  }

  switchModalAltPage = (p) => {
    this.setState({ modalAltPage: p });
  }

  changeAnalyticsDisplay = (start, end, p) => {
    this.setState({ a2Start: start });
    this.setState({ a2End: end });
    this.setState({ aPage: p });
  }

  render() {
    const { posts, analytics1, analytics2, analytics3, updateFormOpen, deleteFormOpen, analyticsOpen, modalAltPage, a2Start, a2End, aPage } = this.state;
    return (
      <div>
        { posts.heroes ? 
          posts.heroes.map(post =>
            <div>
              <div className={styles.rowButtonWrapper}>
                <div className={styles.tableRow}>
                  <div className={Herostyles.tableColumnOne}>
                    {this.rank++}
                  </div>
                  <div className={`${styles.mainColumn} ${Herostyles.tableColumnTwo}`}>
                    {post.name}
                  </div>
                  <div className={Herostyles.tableColumnThree}>
                    {post.win_rate}
                  </div>
                  <div className={Herostyles.tableColumnFour}>
                    {post.pick_rate}
                  </div>
                </div>
                <div className={styles.inRowButtonWrapper}>
                  <div 
                    onClick={() => this.openAnalytics(post)}
                    className={styles.button}
                  >
                    <div className={styles.buttonIcon}><IoStatsChart /></div>
                  </div>
                  <div 
                    onClick={() => this.openUpdate(post)}
                    className={styles.button}
                  >
                    <div className={styles.buttonIcon}><MdBuild /></div>
                  </div>
                  <div 
                    onClick={() => this.openDelete(post)}
                    className={styles.button}
                  >
                    <div className={styles.buttonIcon}><MdDelete /></div>
                  </div>
                </div>
              </div>
            </div>
          )
          : null
        }
        <Modal 
          show={updateFormOpen}
          handler={this.openUpdate}
          header="Update Hero?"
        >
          <div>
            <div className={styles.sectionTitle}>
              SELECTED HERO
            </div>
            <div className={styles.selected}>
              {this.selected.heroName}
            </div>
          </div>
          <form 
            className={styles.formContent}
            onSubmit={this.updateHero}
          >
            <div className={styles.formSection}>
              <div className={styles.sectionTitle}>
                WIN RATE (%)
              </div>
              <input required
                type="text" 
                pattern="[0-9]+.[0-9]+"
                title="Input only numbers as a percentage."
                value={this.selected.winRate}
                onChange={this.handleChangeWinRate}
              />
            </div>
            <div className={styles.formSection}>
              <div className={styles.sectionTitle}>
                PICK RATE (%)
              </div>
              <input required
                type="text" 
                pattern="0.[0-9]+"
                title="Input only numbers as a percentage."
                value={this.selected.pickRate}
                onChange={this.handleChangePickRate}
              />
            </div>
          </form>
          <div onClick={this.updateHero} className={Modalstyles.button}>
            CONFIRM UPDATE
          </div>
        </Modal>
        <Modal 
          show={deleteFormOpen}
          handler={this.openDelete}
          header="Delete Hero?"
        >
          <div>
            <div className={styles.sectionTitle}>
              SELECTED HERO
            </div>
            <div className={styles.selected}>
              {this.selected.heroName}
            </div>
          </div>
          <div onClick={() => this.deleteHero()} className={Modalstyles.button}>
            CONFIRM DELETE
          </div>
        </Modal>
        <ModalAlt 
          show={analyticsOpen}
          handler={this.openAnalytics}
          header={this.selected.heroName}
          subtitle="ANALYTICS"
        >
          <div className={Modalstyles.togglePageButtonWrapper}>
            <div 
              onClick={() => this.switchModalAltPage(1)}
              className={modalAltPage === 1 ? `${Modalstyles.togglePageButtonSelected}` : `${Modalstyles.togglePageButton}`}
            >
              Win % Over Time in Matches
            </div>
            <div 
              onClick={() => this.switchModalAltPage(2)}
              className={modalAltPage === 2 ? `${Modalstyles.togglePageButtonSelected}` : `${Modalstyles.togglePageButton}`}
            >
              Win % Against Other Heroes
            </div>
            <div 
              onClick={() => this.switchModalAltPage(3)}
              className={modalAltPage === 3 ? `${Modalstyles.togglePageButtonSelected}` : `${Modalstyles.togglePageButton}`}
            >
              XP and Gold Comparison
            </div>
          </div>
          <div 
            className={modalAltPage !== 1 ? `${styles.hidden}` : null}
          >
            { analytics1.winrates ? 
              analytics1.winrates.map(a =>
                this.selected.id === a.id ?
                <div>
                  <div className={styles.subSectionTitle}>
                    Win % Over Time in Matches
                  </div>
                  <div className={styles.chart}>
                    <Line
                      data={
                        {
                          labels: ['0', '< 20', '> 20', '> 30', '> 40', '> 50', '> 60'],
                          datasets: [{
                            fill: true,
                            label: ' WIN %',
                            backgroundColor: '#7c86a14b',
                            borderColor: '#7c86a1',
                            borderWidth: 3,
                            pointRadius: 1.5,
                            pointHoverRadius: 1.5,
                            data: [0, a.wless20, a.wmore20, a.wmore30, a.wmore40, a.wmore50, a.wmore60]
                          }]
                        }
                      }
                      options={options1}
                    />
                  </div>
                </div>
                : null
              )
              : null
            }
          </div>
          
          <div 
            className={modalAltPage !== 2 ? `${styles.hidden}` : null}
          >
            {
              analytics2.winrates_vs_heroes ? 
              analytics2.winrates_vs_heroes.map(a =>
                this.selected.id === a.id ?
                <div>
                  <div className={styles.subSectionTitle}>
                    Win % Against Other Heroes
                  </div>
                  <div className={Modalstyles.togglePageButtonWrapper}>
                    <div className={a.vs.heroes.length <= 0 ? `${styles.hidden}` : `${Modalstyles.togglePageNumber}` }>
                      <div 
                        className={aPage === 1 ? `${Modalstyles.togglePageButtonSelected}` : `${Modalstyles.togglePageButton}`}
                        onClick={() => this.changeAnalyticsDisplay(0,10,1)}
                      >
                        1 - {a.vs.heroes.length < 10 ? a.vs.heroes.length : "10"}
                      </div>
                    </div>
                    <div className={a.vs.heroes.length <= 10 ? `${styles.hidden}` : `${Modalstyles.togglePageNumber}` }>
                      <div 
                        className={aPage === 2 ? `${Modalstyles.togglePageButtonSelected}` : `${Modalstyles.togglePageButton}`}
                        onClick={() => this.changeAnalyticsDisplay(10,20,2)}
                      >
                        11 - {a.vs.heroes.length < 20 ? a.vs.heroes.length : "20"}
                      </div>
                    </div>
                    <div className={a.vs.heroes.length <= 20 ? `${styles.hidden}` : `${Modalstyles.togglePageNumber}` }>
                      <div 
                        className={aPage === 3 ? `${Modalstyles.togglePageButtonSelected}` : `${Modalstyles.togglePageButton}`}
                        onClick={() => this.changeAnalyticsDisplay(20,30,3)}
                      >
                        21 - {a.vs.heroes.length < 30 ? a.vs.heroes.length : "30"}
                      </div>
                    </div>
                    <div className={a.vs.heroes.length <= 30 ? `${styles.hidden}` : `${Modalstyles.togglePageNumber}` }>
                      <div 
                        className={aPage === 4 ? `${Modalstyles.togglePageButtonSelected}` : `${Modalstyles.togglePageButton}`}
                        onClick={() => this.changeAnalyticsDisplay(30,40,4)}
                      >
                        31 - {a.vs.heroes.length < 40 ? a.vs.heroes.length : "40"}
                      </div>
                    </div>
                    <div className={a.vs.heroes.length <= 40 ? `${styles.hidden}` : `${Modalstyles.togglePageNumber}` }>
                      <div 
                        className={aPage === 5 ? `${Modalstyles.togglePageButtonSelected}` : `${Modalstyles.togglePageButton}`}
                        onClick={() => this.changeAnalyticsDisplay(40,50,5)}
                      >
                        41 - {a.vs.heroes.length < 50 ? a.vs.heroes.length : "50"}
                      </div>
                    </div>
                    <div className={a.vs.heroes.length <= 50 ? `${styles.hidden}` : `${Modalstyles.togglePageNumber}` }>
                      <div 
                        className={aPage === 6 ? `${Modalstyles.togglePageButtonSelected}` : `${Modalstyles.togglePageButton}`}
                        onClick={() => this.changeAnalyticsDisplay(50,60,6)}
                      >
                        51 - {a.vs.heroes.length < 60 ? a.vs.heroes.length : "60"}
                      </div>
                    </div>
                    <div className={a.vs.heroes.length <= 60 ? `${styles.hidden}` : `${Modalstyles.togglePageNumber}` }>
                      <div 
                        className={aPage === 7 ? `${Modalstyles.togglePageButtonSelected}` : `${Modalstyles.togglePageButton}`}
                        onClick={() => this.changeAnalyticsDisplay(60,70,7)}
                      >
                        61 - {a.vs.heroes.length < 70 ? a.vs.heroes.length : "70"}
                      </div>
                    </div>
                    <div className={a.vs.heroes.length <= 70 ? `${styles.hidden}` : `${Modalstyles.togglePageNumber}` }>
                      <div 
                        className={aPage === 8 ? `${Modalstyles.togglePageButtonSelected}` : `${Modalstyles.togglePageButton}`}
                        onClick={() => this.changeAnalyticsDisplay(70,80,8)}
                      >
                        71 - {a.vs.heroes.length < 80 ? a.vs.heroes.length : "80"}
                      </div>
                    </div>
                    <div className={a.vs.heroes.length <= 80 ? `${styles.hidden}` : `${Modalstyles.togglePageNumber}` }>
                      <div 
                        className={aPage === 9 ? `${Modalstyles.togglePageButtonSelected}` : `${Modalstyles.togglePageButton}`}
                        onClick={() => this.changeAnalyticsDisplay(80,90,9)}
                      >
                        81 - {a.vs.heroes.length < 90 ? a.vs.heroes.length : "90"}
                      </div>
                    </div>
                    <div className={a.vs.heroes.length <= 90 ? `${styles.hidden}` : `${Modalstyles.togglePageNumber}` }>
                      <div 
                      className={aPage === 10 ? `${Modalstyles.togglePageButtonSelected}` : `${Modalstyles.togglePageButton}`}
                      onClick={() => this.changeAnalyticsDisplay(90,100,10)}
                      >
                        91 - {a.vs.heroes.length < 100 ? a.vs.heroes.length : "100"}
                      </div>
                    </div>
                    <div className={a.vs.heroes.length <= 100 ? `${styles.hidden}` : `${Modalstyles.togglePageNumber}` }>
                      <div 
                      className={aPage === 11 ? `${Modalstyles.togglePageButtonSelected}` : `${Modalstyles.togglePageButton}`}
                      onClick={() => this.changeAnalyticsDisplay(100,110,11)}
                      >
                        101 - {a.vs.heroes.length < 110 ? a.vs.heroes.length : "110"}
                      </div>
                    </div>
                    <div className={a.vs.heroes.length <= 110 ? `${styles.hidden}` : `${Modalstyles.togglePageNumber}` }>
                      <div 
                      className={aPage === 12 ? `${Modalstyles.togglePageButtonSelected}` : `${Modalstyles.togglePageButton}`}
                      onClick={() => this.changeAnalyticsDisplay(110,120,12)}
                      >
                        111 - {a.vs.heroes.length < 120 ? a.vs.heroes.length : "120"}
                      </div>
                    </div>
                    
                  </div>
                  <div className={styles.chartAlt}>
                    <HorizontalBar
                      height={240}
                      data={
                        {
                          labels: a.vs.heroes.slice(a2Start,a2End),
                          datasets: [{
                            label: ' WIN % RATIO',
                            backgroundColor: '#7c86a14b',
                            borderColor: '#7c86a1',
                            borderWidth: 2,
                            minBarLength: 5,
                            data: a.vs.winrates.slice(a2Start,a2End),
                          }]
                        }
                      }
                      options={options2}
                    />
                  </div>
                </div>
                : null
              )
              : null
            }
          </div>

          <div 
            className={modalAltPage !== 3 ? `${styles.hidden}` : null}
          >
            { analytics3.gold_xp_comparison ? 
              analytics3.gold_xp_comparison.map(a =>
                this.selected.id === a.hero_id ?
                <div>
                  <div className={styles.subSectionTitle}>
                    XP and Gold Comparison
                  </div>
                  <div className={styles.chart}>
                    <Bar
                      data={
                        {
                          labels: ['XP', 'GOLD'],
                          datasets: [{
                            label: ' AVERAGE PER MINUTE',
                            backgroundColor: ['#7c86a14b', '#af3c3856'],
                            borderColor: ['#7c86a1', '#af3c38'],
                            borderWidth: 2,
                            minBarLength: 5,
                            data: [a.average_xp_per_min, a.average_gold_per_min],
                          }]
                        }
                      }
                      options={options3}
                    />
                  </div>
                </div>
                : null
              )
              : null
            }
          </div>
        </ModalAlt>
      </div>
    );
  }
}