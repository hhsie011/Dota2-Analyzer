import React, { Component } from "react";
import axios from 'axios';
import styles from "../styles/Common.module.css";
import Playerstyles from "../styles/Players.module.css";
import Modalstyles from "../styles/Modal.module.css";

import Modal from "./Modal";

import { MdBuild, MdDelete } from "react-icons/md";

const api = axios.create({ baseURL: 'http://localhost:8080/'});

export default class Player extends Component {
  constructor() {
    super();
    this.state = {  
      posts: [], 
      accountID: 0,
      winCnt: 0,
      matchCnt: 0,
      updateFormOpen: false,
      deleteFormOpen: false,
      analyticsOpen: false
    };
    this.rank = 1;
    this.selected = { 
      id: 0,
      winCnt: 0,
      matchCnt: 0
    };
    this.handleChangeWinCnt = this.handleChangeWinCnt.bind(this);
    this.handleChangeMatchCnt = this.handleChangeMatchCnt.bind(this);
    this.updatePlayer = this.updatePlayer.bind(this);
    this.deletePlayer = this.deletePlayer.bind(this);
  }

  componentDidUpdate() {
    this.rank = 1;
  }

  componentDidMount(){
    axios.get('./../players.json')
      .then(res => {
          console.log(res);
          this.setState({posts: res.data});
      })
      .catch(err => { console.log(err); })
  }

  openUpdate = (post) => {
    this.setState({ updateFormOpen: !this.state.updateFormOpen });
    this.selected.id = post.id;
    this.selected.winCnt = post.total_wins;
    this.selected.matchCnt = post.total_matches;
  }

  openDelete = (post) => {
    this.setState({ deleteFormOpen: !this.state.deleteFormOpen });
    this.selected.id = post.id;
  }

  handleChangeWinCnt(e) {
    this.setState({ winCnt: e.target.value });
    this.selected.winCnt = e.target.value;
  }
  handleChangeMatchCnt(e) {
    this.setState({ matchCnt: e.target.value });
    this.selected.matchCnt = e.target.value;
  }

  updatePlayer() {
    const { id, winCnt, matchCnt } = this.selected;
    const req = JSON.stringify({
      "request_type": "update_player",
      "account_id": id,
      "win_count": winCnt,
      "match_count": matchCnt
    });
    api.post('/update/player', req, 
      {headers: {"content-type": "application/json"}})
      .then(res => { console.log(res) })
      .catch(err => { console.log(err) })
  }

  deletePlayer() {
    const { id } = this.selected;
    const req = JSON.stringify({
      "request_type": "delete_player",
      "account_id": id
    });
    api.post('/delete/player', req, 
      {headers: {"content-type": "application/json"}})
      .then(res => { console.log(res) })
      .catch(err => { console.log(err) })
  }

  render() {
    const { posts, updateFormOpen, deleteFormOpen } = this.state;
    return (
      <div>
        { posts.players ? 
          posts.players.map(post =>
            <div className={styles.rowButtonWrapper}>
              <div className={styles.tableRow}>
                <div className={Playerstyles.tableColumnOne}>
                  {this.rank++}
                </div>
                <div className={`${styles.mainColumn} ${Playerstyles.tableColumnTwo}`}>
                  {post.id}
                </div>
                <div className={Playerstyles.tableColumnThree}>
                  {post.win_rate}
                </div>
                <div className={Playerstyles.tableColumnFour}>
                  {post.total_wins}
                </div>
                <div className={Playerstyles.tableColumnFive}>
                  {post.total_matches}
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
          header="Update Player?"
        >
          <div>
            <div className={styles.sectionTitle}>
              SELECTED PLAYER
            </div>
            <div className={styles.selected}>
              {this.selected.id}
            </div>
          </div>
          <form 
            className={styles.formContent}
            onSubmit={this.updatePlayer}
          >
            <div className={styles.formSection}>
              <div className={styles.sectionTitle}>
                WIN COUNT
              </div>
              <input required
                type="text" 
                pattern="[0-9]+"
                title="Input only numbers."
                value={this.selected.winCnt}
                onChange={this.handleChangeWinCnt}
              />
            </div>
            <div className={styles.formSection}>
              <div className={styles.sectionTitle}>
                MATCH COUNT
              </div>
              <input required
                type="text" 
                pattern="[0-9]+"
                title="Input only numbers."
                value={this.selected.matchCnt}
                onChange={this.handleChangeMatchCnt}
              />
            </div>
          </form>
          <div onClick={this.updatePlayer} className={Modalstyles.button}>
            CONFIRM UPDATE
          </div>
        </Modal>
        <Modal 
          show={deleteFormOpen}
          handler={this.openDelete}
          header="Delete Player?"
        >
          <div>
            <div className={styles.sectionTitle}>
              SELECTED PLAYER
            </div>
            <div className={styles.selected}>
              {this.selected.id}
            </div>
          </div>
          <div onClick={() => this.deletePlayer()} className={Modalstyles.button}>
            CONFIRM DELETE
          </div>
        </Modal>
      </div>
    );
  }
}
