import React, { Component } from "react";
import styles from "../styles/Common.module.css";
import Modalstyles from "../styles/Modal.module.css";

import { MdCancel } from "react-icons/md";

export default class Modal extends Component {
  render() {
    return (
      <div>
        <div className={this.props.show ? `${Modalstyles.modal} ${Modalstyles.show}` : `${Modalstyles.modal}`}>
          <div className={Modalstyles.content}>
            <div className={styles.formFiller} />
            <div className={Modalstyles.formWrapper}>
              <div className={this.props.show ? `${Modalstyles.showContent}` : `${Modalstyles.hiddenContent}`}>
                
                <div className={styles.formTitle}>
                  {this.props.header}
                </div>

                {this.props.children}

                <div className={Modalstyles.footer}>
                  <div onClick={this.props.handler} className={`${Modalstyles.button} ${Modalstyles.highlight}`} >
                    CANCEL
                  </div>
                </div>

              </div>
            </div>
            <div className={styles.formFillerIcon}>
              <span onClick={this.props.handler} className={Modalstyles.cancelButton}>
                <MdCancel />
              </span>
            </div>
          </div> 
        </div> 
        <div onClick={this.props.handler} className={Modalstyles.overlay} />
      </div>
    );
  }
}