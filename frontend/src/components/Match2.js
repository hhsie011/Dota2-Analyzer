import React, { Component } from "react";
import axios from 'axios';
import styles from "../styles/Common.module.css";
import Matchstyles from "../styles/Matches.module.css";
import Modalstyles from "../styles/Modal.module.css";

import Modal from "./Modal";

import { MdBuild, MdDelete } from "react-icons/md";

const api = axios.create({ baseURL: 'http://localhost:8080/'});

export default class Match2 extends Component {
  constructor() {
    super();
    this.state = {  
      posts: [], 
      matchID: 0,
      winType: "",
      firstBloodTime: 0,
      clusterRegion: "US WEST",
      likes: 0,
      dislikes: 0,
      updateFormOpen: false,
      deleteFormOpen: false
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
    axios.get('./../cluster_region.json')
      .then(res => {
        console.log(res);
        this.setState({posts: res.data});
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
    document.getElementById("clustersCR").value = this.selected.clusterRegion;
  }

  openDelete = (post) => {
    this.setState({ deleteFormOpen: !this.state.deleteFormOpen });
    this.selected.id = post.id;
  }

  render() {
    const { posts, updateFormOpen, deleteFormOpen } = this.state;
    return (
      <div>
        { posts.matches ? 
          posts.matches.map(post =>
            <div className={styles.rowButtonWrapper}>
              <div className={styles.tableRow}>
                <div className={Matchstyles.crColumnOne}>
                  {this.rank++}
                </div>
                <div className={`${styles.mainColumn} ${Matchstyles.crColumnTwo}`}>
                  {post.id}
                </div>
                <div className={Matchstyles.crColumnThree}>
                  {post.region_id}
                </div>
                <div className={Matchstyles.crColumnFour}>
                  {post.region}
                </div>
              </div>
              <div className={styles.inRowButtonWrapper}>
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
            onSubmit={this.insertMatch}
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
                id="clustersCR"
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
      </div>
    );
  }
}
