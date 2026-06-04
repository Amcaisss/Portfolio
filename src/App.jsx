import React, { useState } from "react";
import { useGameState } from "./hooks/useGameState";
import { useBattle } from "./hooks/useBattle";
import Onboarding from "./components/Onboarding";
import TrainerBar from "./components/TrainerBar";
import QuestTab from "./components/QuestTab";
import BattleScreen from "./components/BattleScreen";
import { BagPanel, PokedexPanel } from "./components/BagPokedex";
import "./styles.css";

const App = () => {
  const game = useGameState();
  const {
    state, rewardItems, notification,
    completeQuest, pickRewardItem,
    useItemInBattle, catchPokemon, recordBattleResult,
    setTrainerName, generateDailyQuests, resetGame,
  } = game;

  const battle = useBattle({
    bag: state.bag,
    useItemInBattle,
    catchPokemon,
    recordBattleResult,
  });

  const [tab, setTab] = useState("quests"); // quests | battle | bag | pokedex
  const [showReset, setShowReset] = useState(false);

  if (!state.trainerName) {
    return <Onboarding onStart={setTrainerName} />;
  }

  return (
    <div className="app">
      <div className="scanlines" />

      {/* Notification toast */}
      {notification && <div className="toast pixel-box">{notification}</div>}

      {/* Header */}
      <header className="app-header pixel-box">
        <div className="header-brand">⚡ POKÉTASKS</div>
        <button className="pixel-btn-sm danger" onClick={() => setShowReset(true)}>RESET</button>
      </header>

      {/* Trainer bar */}
      <TrainerBar
        trainerName={state.trainerName}
        wins={state.wins}
        losses={state.losses}
        totalCompleted={state.totalCompleted}
        caughtCount={state.caught.length}
      />

      {/* Tab navigation */}
      <nav className="tab-nav">
        {[
          { id: "quests",  label: "📋 MISSIONS", badge: state.quests.filter(q => !q.completed).length },
          { id: "battle",  label: "⚔️ COMBAT" },
          { id: "bag",     label: "🎒 SAC", badge: Object.values(state.bag).reduce((a,b)=>a+b,0) || null },
          { id: "pokedex", label: "📖 POKÉDEX", badge: state.caught.length || null },
        ].map(({ id, label, badge }) => (
          <button
            key={id}
            className={`tab-btn ${tab === id ? "active" : ""}`}
            onClick={() => setTab(id)}
          >
            {label}
            {badge > 0 && <span className="tab-badge">{badge}</span>}
          </button>
        ))}
      </nav>

      {/* Content */}
      <main className="app-main">
        {tab === "quests" && (
          <QuestTab
            quests={state.quests}
            rewardItems={rewardItems}
            onComplete={completeQuest}
            onPickReward={pickRewardItem}
            onRefresh={() => generateDailyQuests()}
          />
        )}
        {tab === "battle" && (
          <BattleScreen
            battle={battle}
            bag={state.bag}
            onStartBattle={battle.startBattle}
            onAttack={battle.playerAttack}
            onCapture={battle.tryCapture}
            onPotion={battle.usePotion}
            onFlee={battle.flee}
            onEndBattle={battle.endBattle}
          />
        )}
        {tab === "bag" && <BagPanel bag={state.bag} />}
        {tab === "pokedex" && <PokedexPanel caught={state.caught} />}
      </main>

      {/* Reset confirm */}
      {showReset && (
        <div className="modal-overlay" onClick={() => setShowReset(false)}>
          <div className="confirm-modal pixel-box" onClick={e => e.stopPropagation()}>
            <div className="pixel-title">RÉINITIALISER ?</div>
            <p>Toute ta progression sera perdue.</p>
            <div className="confirm-btns">
              <button className="pixel-btn" onClick={() => setShowReset(false)}>NON</button>
              <button className="pixel-btn red" onClick={() => { resetGame(); setShowReset(false); }}>OUI</button>
            </div>
          </div>
        </div>
      )}
    </div>
  );
};

export default App;
