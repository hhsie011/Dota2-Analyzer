import React, { Component } from "react";
import axios from 'axios';
import styles from "../styles/Common.module.css";
import Abilitystyles from "../styles/Abilities.module.css";
import Modalstyles from "../styles/Modal.module.css";

import Modal from "./Modal";

import { MdBuild, MdDelete } from "react-icons/md";

const api = axios.create({ baseURL: 'http://localhost:8080/'});

export default class Ability extends Component {
  constructor() {
    super();
    this.state = {  
      posts: [], 
      upgradeCnt: 0,
      updateFormOpen: false,
      updateFormOpenPassword: false,
      deleteFormOpen: false,
      password: "",
      errorMsg: ""
    };
    this.rank = 1;
    this.selected = { 
      id: 0,
      name: "",
      upgradeCnt: 0
    };
    this.handleChangeUpgradeCnt = this.handleChangeUpgradeCnt.bind(this);
    this.updateAbility = this.updateAbility.bind(this);
    this.deleteAbility = this.deleteAbility.bind(this);
    this.handlePassword = this.handlePassword.bind(this);
  }

  componentDidUpdate() {
    this.rank = 1;
  }

  componentDidMount(){
    axios.get('./../ability_upgrades.json')
      .then(res => {
        console.log(res);
        this.setState({posts: res.data});
      })
      .catch(err => { console.log(err); })
  }

  handleChangeUpgradeCnt(e) {
    this.setState({ upgradeCnt: e.target.value });
    this.selected.upgradeCnt = e.target.value;
  }

  handlePassword(e) {
    this.setState({ password: e.target.value });
  }

  updateAbility() {
    const { id, upgradeCnt } = this.selected;
    const req = JSON.stringify({
      "request_type": "update_ability",
      "ability_id": id,
      "upgrade_count": upgradeCnt
    });
    api.post('/update/ability', req, 
      {headers: {"content-type": "application/json"}})
      .then(res => { console.log(res) })
      .catch(err => { console.log(err) })
  }

  deleteAbility() {
    const { id } = this.selected;
    const req = JSON.stringify({
      "request_type": "delete_ability",
      "ability_id": id
    });
    api.post('/delete/ability', req, 
      {headers: {"content-type": "application/json"}})
      .then(res => { console.log(res) })
      .catch(err => { console.log(err) })
  }

  openPassword = (post) => {
    this.setState({ 
      updateFormOpenPassword: !this.state.updateFormOpenPassword,
      password: "",
      errorMsg: ""
    });
    this.selected.id = post.id;
    this.selected.name = post.name;
    this.selected.upgradeCnt = post.numTimes_upgraded;
  }

  openUpdate = () => {
    if (this.state.password === "1234") {
      this.setState({ 
        updateFormOpenPassword: !this.state.updateFormOpenPassword,
        updateFormOpen: !this.state.updateFormOpen,
        password: ""
      });
    } 
    else if (this.state.updateFormOpen) {
      this.setState({ updateFormOpen: !this.state.updateFormOpen });
    }
    else {
      this.setState({ errorMsg: "ERROR: Wrong password! Try again." });
    }
  }

  openDelete = (post) => {
    this.setState({ deleteFormOpen: !this.state.deleteFormOpen });
    this.selected.id = post.id;
    this.selected.name = post.name;
  }

  render() {
    const { posts, updateFormOpen, updateFormOpenPassword, deleteFormOpen, password } = this.state;
    return (
      <div>
        { posts.abilities_upgraded ? 
          posts.abilities_upgraded.map(post =>
            <div className={styles.rowButtonWrapper}>
              <div className={styles.tableRow}>
                <div className={Abilitystyles.tableColumnOne}>
                  {this.rank++}
                </div>
                <div className={`${styles.mainColumn} ${Abilitystyles.tableColumnTwo}`}>
                  {post.name}
                </div>
                <div className={Abilitystyles.tableColumnThree}>
                  {post.numTimes_upgraded}
                </div>
              </div>
              <div className={styles.inRowButtonWrapper}>
                <div 
                  onClick={() => this.openPassword(post)}
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
          show={updateFormOpenPassword}
          handler={this.openPassword}
          header="Please Enter Password"
        >
          <div>
            <div className={Modalstyles.alert}>
              {this.state.errorMsg}
            </div>
            <div className={styles.sectionTitle}>
              PASSWORD
            </div>
          </div>
          <form 
            className={styles.formContent}
            onSubmit={this.openUpdate}
          >
            <div className = {styles.formContent}>
              <input required
                type="text" 
                value={password}
                onChange={this.handlePassword}
              />
            </div>
          </form>
          <div onClick={this.openUpdate} className={Modalstyles.button}>
            CONFIRM 
          </div>
        </Modal>
        <Modal 
          show={updateFormOpen}
          handler={this.openUpdate}
          header="Update Ability?"
        >
          <div>
            <div className={styles.sectionTitle}>
              SELECTED ABILITY
            </div>
            <div className={styles.selected}>
              {this.selected.name}
            </div>
          </div>
          <form 
            className={styles.formContent}
            onSubmit={this.updateAbility}
          >
            <div className={styles.formSection}>
              <div className={styles.sectionTitle}>
                UPGRADE COUNT
              </div>
              <input required
                type="text" 
                pattern="0.[0-9]+"
                title="Input only numbers as a percentage."
                value={this.selected.upgradeCnt}
                onChange={this.handleChangeUpgradeCnt}
              />
            </div>
          </form>
          <div onClick={this.updateAbility} className={Modalstyles.button}>
            CONFIRM UPDATE
          </div>
        </Modal>
        <Modal 
          show={deleteFormOpen}
          handler={this.openDelete}
          header="Delete Ability?"
        >
          <div>
            <div className={styles.sectionTitle}>
              SELECTED ABILITY
            </div>
            <div className={styles.selected}>
              {this.selected.name}
            </div>
          </div>
          <div onClick={() => this.deleteAbility()} className={Modalstyles.button}>
            CONFIRM DELETE
          </div>
        </Modal>
      </div>
    );
  }
}
