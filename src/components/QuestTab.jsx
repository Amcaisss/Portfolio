import React from "react";
import { ITEMS } from "../data/pokemon";

const QuestTab = ({ quests, rewardItems, onComplete, onPickReward, onRefresh }) => {
  const completed = quests.filter((q) => q.completed).length;

  return (
    <div className="quest-tab">
      <div className="quest-header pixel-box">
        <div className="quest-title-row">
          <span className="pixel-title">📋 MISSIONS DU JOUR</span>
          <button className="pixel-btn-sm" onClick={onRefresh}>↺ NOUVELLES</button>
        </div>
        <div className="quest-progress-bar">
          <div
            className="quest-progress-fill"
            style={{ width: `${quests.length ? (completed / quests.length) * 100 : 0}%` }}
          />
        </div>
        <div className="quest-progress-label">{completed} / {quests.length} complétées</div>
      </div>

      <div className="quest-list">
        {quests.map((quest) => (
          <div
            key={quest.id}
            className={`quest-item pixel-box ${quest.completed ? "quest-done" : ""}`}
          >
            <div className="quest-checkbox">
              {quest.completed ? "✅" : "⬜"}
            </div>
            <div className="quest-text">{quest.text}</div>
            {!quest.completed && (
              <button
                className="pixel-btn-sm green"
                onClick={() => onComplete(quest.id)}
              >
                ► FAIT !
              </button>
            )}
            {quest.completed && (
              <div className="quest-reward-badge">+1 OBJET</div>
            )}
          </div>
        ))}
      </div>

      {completed === quests.length && quests.length > 0 && (
        <div className="all-done pixel-box blink-border">
          🏆 TOUTES LES MISSIONS ACCOMPLIES !
        </div>
      )}

      {/* Reward item picker modal */}
      {rewardItems && (
        <div className="reward-overlay">
          <div className="reward-modal pixel-box">
            <div className="reward-title">🎁 CHOISIS TON RÉCOMPENSE !</div>
            <div className="reward-subtitle">Sélectionne 1 objet parmi les 3 :</div>
            <div className="reward-items">
              {rewardItems.map((item) => (
                <button
                  key={item.id}
                  className="reward-item-btn pixel-box"
                  onClick={() => onPickReward(item.id)}
                >
                  <div className="reward-item-icon">{item.icon}</div>
                  <div className="reward-item-name">{item.name}</div>
                  <div className="reward-item-rarity" data-rarity={item.rarity}>
                    {item.rarity}
                  </div>
                  <div className="reward-item-desc">{item.description}</div>
                </button>
              ))}
            </div>
          </div>
        </div>
      )}
    </div>
  );
};

export default QuestTab;
