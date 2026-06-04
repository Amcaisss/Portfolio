import React, { useState } from "react";

const Onboarding = ({ onStart }) => {
  const [name, setName] = useState("");
  const [leaving, setLeaving] = useState(false);

  const handleStart = () => {
    if (!name.trim()) return;
    setLeaving(true);
    setTimeout(() => onStart(name.trim()), 600);
  };

  return (
    <div className={`onboarding ${leaving ? "leaving" : ""}`}>
      <div className="ob-scanlines" />
      <div className="ob-content">
        <div className="ob-logo">
          <div className="logo-pixel">⚡</div>
          <div className="logo-title">POKÉTASKS</div>
          <div className="logo-sub">DAILY QUEST EDITION</div>
        </div>

        <div className="ob-professor">
          <div className="professor-sprite">🥼</div>
          <div className="speech-bubble pixel-box">
            <p>Bonjour ! Je suis le Professeur Chen.</p>
            <p>Chaque jour, accomplis tes missions pour gagner des objets et partir à l'aventure !</p>
          </div>
        </div>

        <div className="ob-form pixel-box">
          <label className="pixel-label">TON NOM, DRESSEUR :</label>
          <input
            className="pixel-input"
            type="text"
            value={name}
            onChange={(e) => setName(e.target.value)}
            onKeyDown={(e) => e.key === "Enter" && handleStart()}
            placeholder="Sacha..."
            maxLength={12}
            autoFocus
          />
          <button
            className={`pixel-btn ${name.trim() ? "active" : ""}`}
            onClick={handleStart}
            disabled={!name.trim()}
          >
            ► COMMENCER
          </button>
        </div>

        <div className="ob-features">
          <span className="pixel-tag">📋 Quêtes quotidiennes</span>
          <span className="pixel-tag">🎒 Sac à objets</span>
          <span className="pixel-tag">⚔️ Combats</span>
          <span className="pixel-tag">📖 Pokédex</span>
        </div>
      </div>
    </div>
  );
};

export default Onboarding;
