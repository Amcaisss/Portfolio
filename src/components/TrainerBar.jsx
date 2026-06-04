import React from "react";

const TrainerBar = ({ trainerName, wins, losses, totalCompleted, caughtCount }) => {
  return (
    <div className="trainer-bar pixel-box">
      <div className="trainer-left">
        <div className="trainer-avatar">🧢</div>
        <div className="trainer-info">
          <div className="trainer-name">{trainerName}</div>
          <div className="trainer-title">DRESSEUR POKÉMON</div>
        </div>
      </div>
      <div className="trainer-stats">
        <div className="t-stat">
          <div className="t-stat-val">{totalCompleted}</div>
          <div className="t-stat-lbl">Missions</div>
        </div>
        <div className="t-stat">
          <div className="t-stat-val">{wins}</div>
          <div className="t-stat-lbl">Victoires</div>
        </div>
        <div className="t-stat">
          <div className="t-stat-val">{caughtCount}</div>
          <div className="t-stat-lbl">Capturés</div>
        </div>
      </div>
    </div>
  );
};

export default TrainerBar;
