import React, { Component } from "react";
import { Bar } from "react-chartjs-2";
import axios from 'axios';
import styles from "../styles/Common.module.css";
import Matchstyles from "../styles/Matches.module.css";
import Modalstyles from "../styles/Modal.module.css";

import Modal from "./Modal";
import ModalAlt from "./Modal2";

import { MdBuild, MdDelete } from "react-icons/md";
import { IoStatsChart } from "react-icons/io5";

const api = axios.create({ baseURL: 'http://localhost:8080/'});

const options = {
  responsive: true,
  legend: { 
    display: true,
    labels: {
      fontColor: '#7c86a1'
    }
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
        labelString: 'WIN RATE (%)',
        fontColor: '#49526b',
        fontSize: 16,
        fontStyle: '600'
      },
    }]
  },
}
export default class Match extends Component {
  constructor() {
    super();
    this.state = {  
      posts: [], 
      analytics: [],
      matchID: 0,
      winType: "",
      firstBloodTime: 0,
      clusterRegion: "US WEST",
      likes: 0,
      dislikes: 0,
      updateFormOpen: false,
      deleteFormOpen: false,
      analyticsOpen: false
    };
    this.rank = 1;
    this.selected = { 
      id: 0,
      winType: "",
      firstBloodTime: 0,
      clusterRegion: "US WEST",
      likes: 0,
      dislikes: 0
    };
    this.handleChangeMatchID = this.handleChangeMatchID.bind(this);
    this.handleChangeWinType = this.handleChangeWinType.bind(this);
    this.handleChangeFBTime = this.handleChangeFBTime.bind(this);
    this.handleChangeClusterRegion = this.handleChangeClusterRegion.bind(this);
    this.handleChangeLikes = this.handleChangeLikes.bind(this);
    this.handleChangeDislikes = this.handleChangeDislikes.bind(this);
    this.updateMatch = this.updateMatch.bind(this);
    this.deleteMatch = this.deleteMatch.bind(this);
  }

  componentDidUpdate() {
    this.rank = 1;
  }

  componentDidMount(){
    axios.get('./../first_blood.json')
      .then(res => {
        console.log(res);
        this.setState({posts: res.data});
      })
      .catch(err => { console.log(err); })
    axios.get('./../first_blood_winrate.json')
      .then(res => {
        console.log(res);
        this.setState({analytics: res.data});
      })
      .catch(err => { console.log(err); })
  }

  handleChangeMatchID(e) {
    this.setState({ matchID: e.target.value });
    this.selected.id = e.target.value;
  }
  handleChangeWinType(e) {
    this.setState({ winType: e.target.value });
    this.selected.winType = e.target.value;
  }
  handleChangeFBTime(e) {
    this.setState({ firstBloodTime: e.target.value });
    this.selected.firstBloodTime = e.target.value;
  }
  handleChangeClusterRegion(e) {
    this.setState({ clusterRegion: e.target.value });
    this.selected.clusterRegion = e.target.value;
  }
  handleChangeLikes(e) {
    this.setState({ likes: e.target.value });
    this.selected.likes = e.target.value;
  }
  handleChangeDislikes(e) {
    this.setState({ dislikes: e.target.value });
    this.selected.dislikes = e.target.value;
  }

  updateMatch() {
    const { id, firstBloodTime, winType, likes, dislikes, clusterRegion } = this.selected;
    const req = JSON.stringify({
      "request_type": "update_match",
      "match_id": id,
      "win_type": winType,
      "first_blood_time": firstBloodTime,
      "cluster_region": clusterRegion,
      "likes": likes,
      "dislikes": dislikes
    });
    api.post('/update/match', req, 
      {headers: {"content-type": "application/json"}})
      .then(res => { console.log(res) })
      .catch(err => { console.log(err) })
  }

  deleteMatch() {
    const { id } = this.selected;
    const req = JSON.stringify({
      "request_type": "delete_match",
      "match_id": id
    });
    api.post('/delete/match', req, 
      {headers: {"content-type": "application/json"}})
      .then(res => { console.log(res) })
      .catch(err => { console.log(err) })
  }
  openUpdate = (post) => {
    this.setState({ updateFormOpen: !this.state.updateFormOpen });
    if (!this.state.updateFormOpen) {
      this.selected.id = post.id;
      this.selected.winType = post.win_type;
      this.selected.firstBloodTime = post.first_blood_time;
      this.selected.clusterRegion = post.region;
      this.selected.likes = post.likes;
      this.selected.dislikes = post.dislikes;
    }
    else {
      this.selected.id = 0;
      this.selected.winType = "";
      this.selected.firstBloodTime = 0;
      this.selected.clusterRegion = "US WEST";
      this.selected.likes = 0;
      this.selected.dislikes = 0;
    }
    
    document.getElementById("clustersFB").value = this.selected.clusterRegion;
  }

  openDelete = (post) => {
    this.setState({ deleteFormOpen: !this.state.deleteFormOpen });
    this.selected.id = post.id;
  }

  getFirstBloodWinRate = (post) => {
    this.setState({ analyticsOpen: !this.state.analyticsOpen });
    this.selected.id = post.id;
    this.selected.winType = post.win_type;
    this.selected.firstBloodTime = post.first_blood_time;
  }

  render() {
    const { posts, analytics, updateFormOpen, deleteFormOpen, analyticsOpen } = this.state;
    return (
      <div>
        { posts.matches ? 
          posts.matches.map(post =>
            <div className={styles.rowButtonWrapper}>
              <div className={styles.tableRow}>
                <div className={Matchstyles.fbColumnOne}>
                  {this.rank++}
                </div>
                <div className={`${styles.mainColumn} ${Matchstyles.fbColumnTwo}`}>
                  {post.id}
                </div>
                <div className={Matchstyles.fbColumnThree}>
                  {post.win_type}
                </div>
                <div className={Matchstyles.fbColumnFour}>
                  {post.first_blood_time}
                </div>
              </div>
              <div className={styles.inRowButtonWrapper}>
                <div 
                  onClick={() => this.getFirstBloodWinRate(post)}
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
          )
          : null
        }
        <Modal 
          show={updateFormOpen}
          handler={this.openUpdate}
          header="Update Match?"
        >
          <div>
            <div className={styles.sectionTitle}>
              SELECTED MATCH
            </div>
            <div className={styles.selected}>
              {this.selected.id}
            </div>
          </div>
          <form 
            className={styles.formContent}
            onSubmit={this.updateMatch}
          >
            <div className={styles.formSection}>
              <div className={styles.sectionTitle}>
                WIN TYPE 
              </div>
              <input required
                type="text" 
                pattern="(Radiant)||(Dire)"
                title={"Input either \"Radiant\" or \"Dire.\""}
                value={this.selected.winType}
                onChange={this.handleChangeWinType}
              />
            </div>
            <div className={styles.formSection}>
              <div className={styles.sectionTitle}>
                FIRST BLOOD TIME
              </div>
              <input required
                type="text" 
                pattern="[0-9]+"
                title="Input only numbers."
                value={this.selected.firstBloodTime}
                onChange={this.handleChangeFBTime}
              />
            </div>
            <div className={styles.formSection}>
              <div className={styles.sectionTitle}>
                CLUSTER REGION
              </div>
              <select 
                onChange={this.handleChangeClusterRegion}
                id="clustersFB"
              >
                <option value="US WEST">US WEST</option>
                <option value="US EAST">US EAST</option>
                <option value="EUROPE">EUROPE</option>
                <option value="JAPAN">JAPAN</option>
                <option value="SINGAPORE">SINGAPORE</option>
                <option value="DUBAI">DUBAI</option>
                <option value="AUSTRALIA">AUSTRALIA</option>
                <option value="STOCKHOLM">STOCKHOLM</option>
                <option value="AUSTRIA">AUSTRIA</option>
                <option value="BRAZIL">BRAZIL</option>
                <option value="SOUTHAFRICA">SOUTH AFRICA</option>
                <option value="PW TELECOM ZHEJIANG">PW TELECOM ZHEJIANG</option>
                <option value="PW TELECOM SHANGHAI">PW TELECOM SHANGHAI</option>
                <option value="PW TELECOM GUANGDONG">PW TELECOM GUANGDONG</option>
                <option value="PW TELECOM WUHAN">PW TELECOM WUHAN</option>
                <option value="PW UNICOM">PW UNICOM</option>
                <option value="PW UNICOM TIANJIN">PW UNICOM TIANJIN</option>
                <option value="CHILE">CHILE</option>
                <option value="PERU">PERU</option>
                <option value="INDIA">INDIA</option>
              </select>
            </div>
            <div className={styles.formSection}>
              <div className={styles.sectionTitle}>
                LIKE COUNT
              </div>
              <input required
                type="text" 
                pattern="[0-9]+"
                title="Input only numbers."
                value={this.selected.likes}
                onChange={this.handleChangeLikes}
              />
            </div>
            <div className={styles.formSection}>
              <div className={styles.sectionTitle}>
                DISLIKE COUNT
              </div>
              <input required
                type="text" 
                pattern="[0-9]+"
                title="Input only numbers."
                value={this.selected.dislikes}
                onChange={this.handleChangeDislikes}
              />
            </div>
          </form>
          <div onClick={this.updateMatch} className={Modalstyles.button}>
            CONFIRM UPDATE 
          </div>
        </Modal>
        <Modal 
          show={deleteFormOpen}
          handler={this.openDelete}
          header="Delete Match?"
        >
          <div>
            <div className={styles.sectionTitle}>
              SELECTED MATCH
            </div>
            <div className={styles.selected}>
              {this.selected.id}
            </div>
          </div>
          <div onClick={this.deleteMatch} className={Modalstyles.button}>
            CONFIRM DELETE
          </div>
        </Modal>
        <ModalAlt 
          show={analyticsOpen}
          handler={this.getFirstBloodWinRate}
          header={"MATCH #" + this.selected.id}
          subtitle="ANALYTICS"
        >
          { analytics.winrates ? 
            analytics.winrates.map(a =>
              this.selected.id === a.id ?
              <div>
                <div className={styles.subSectionTitle}>
                  WIN % FOR FIRST BLOOD
                </div>
                <div className={styles.chartAlt}>
                  <Bar
                    height={240}
                    data={
                      {
                        labels: ['FIRST BLOOD'],
                        datasets: [{
                          label: ' GENERAL',
                          backgroundColor: '#7c86a14b',
                          borderColor: '#7c86a1',
                          borderWidth: 2,
                          minBarLength: 5,
                          data: [a.general],
                        },
                        {
                          label: ' ' + this.selected.winType.toUpperCase(),
                          backgroundColor: '#af3c3856',
                          borderColor: '#af3c38',
                          borderWidth: 2,
                          minBarLength: 5,
                          data: [a.specific],
                        }]
                      }
                    }
                    options={options}
                  />
                </div>
              </div>
              : null
            )
            : null
          }
        </ModalAlt>
      </div>
    );
  }
}
