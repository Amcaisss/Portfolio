import React, { useState } from "react";
import { TYPE_COLORS, ITEMS } from "../data/pokemon";

const HP_BAR_COLOR = (pct) => {
  if (pct > 0.5) return "#4caf50";
  if (pct > 0.25) return "#ff9800";
  return "#f44336";
};

const PokemonSprite = ({ pokemonId, isWild, fainted }) => {
  const spriteBase = isWild
    ? `https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/${pokemonId}.png`
    : `https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/back/1.png`;

  return (
    <img
      src={spriteBase}
      alt=""
      className={`pokemon-sprite ${isWild ? "wild-sprite" : "player-sprite"} ${fainted ? "fainted" : ""}`}
      style={{ imageRendering: "pixelated" }}
      onError={(e) => { e.target.style.display = "none"; }}
    />
  );
};

const HpBar = ({ current, max, label }) => {
  const pct = current / max;
  return (
    <div className="hp-bar-wrap">
      <span className="hp-label">{label} HP</span>
      <div className="hp-bar">
        <div
          className="hp-bar-fill"
          style={{ width: `${pct * 100}%`, background: HP_BAR_COLOR(pct) }}
        />
      </div>
      <span className="hp-numbers">{current}/{max}</span>
    </div>
  );
};

const BattleScreen = ({
  battle,
  bag,
  onStartBattle,
  onAttack,
  onCapture,
  onPotion,
  onFlee,
  onEndBattle,
}) => {
  const [subMenu, setSubMenu] = useState(null); // null | "bag" | "balls"

  const { wildPokemon, wildHp, playerHp, playerMaxHp, log, turn, battleOver, phase } = battle;

  const balls = ITEMS.filter((i) => i.catchRate).filter((i) => bag[i.id] > 0);
  const potions = ITEMS.filter((i) => i.heal).filter((i) => bag[i.id] > 0);

  if (phase === "idle") {
    return (
      <div className="battle-idle pixel-box">
        <div className="idle-art">
          <div className="idle-grass">🌿🌿🌿🌿🌿🌿🌿🌿</div>
          <div className="idle-scene">Route Sauvage</div>
          <div className="idle-desc">Des Pokémon sauvages rôdent dans les hautes herbes...</div>
        </div>
        <button className="pixel-btn active large" onClick={onStartBattle}>
          ► ENTRER DANS LES HERBES
        </button>
      </div>
    );
  }

  return (
    <div className="battle-screen">
      {/* Wild pokemon info */}
      <div className="battle-field pixel-box">
        <div className="battle-enemy-info">
          <div className="battle-name-row">
            <span className="battle-pokemon-name">{wildPokemon?.name}</span>
            {wildPokemon && (
              <span
                className="battle-type-badge"
                style={{ background: TYPE_COLORS[wildPokemon.type] || "#888" }}
              >
                {wildPokemon.type}
              </span>
            )}
            <span className="battle-rarity">{wildPokemon?.rarity}</span>
          </div>
          <HpBar current={wildHp} max={wildPokemon?.hp + 20 || 100} label="Ennemi" />
        </div>

        <div className="battle-sprites">
          <div className="wild-side">
            {wildPokemon && (
              <PokemonSprite pokemonId={wildPokemon.id} isWild fainted={wildHp <= 0} />
            )}
          </div>
          <div className="vs-text">VS</div>
          <div className="player-side">
            <PokemonSprite pokemonId={1} isWild={false} fainted={playerHp <= 0} />
          </div>
        </div>

        <div className="battle-player-info">
          <HpBar current={playerHp} max={playerMaxHp} label="Toi" />
        </div>
      </div>

      {/* Battle log */}
      <div className="battle-log pixel-box">
        {log.slice(-3).map((l, i) => (
          <div key={i} className={`log-line ${i === log.slice(-3).length - 1 ? "log-latest" : ""}`}>
            {l}
          </div>
        ))}
        {(turn === "enemy" || turn === "catching") && !battleOver && (
          <div className="log-line blink">▌</div>
        )}
      </div>

      {/* Battle over */}
      {battleOver ? (
        <div className="battle-result pixel-box">
          {battleOver === "win" && <div className="result-win">🏆 VICTOIRE !</div>}
          {battleOver === "lose" && <div className="result-lose">💀 DÉFAITE...</div>}
          {battleOver === "caught" && <div className="result-caught">🎉 POKÉMON CAPTURÉ !</div>}
          {battleOver === "flee" && <div className="result-flee">🏃 TU T'ES ENFUI !</div>}
          <button className="pixel-btn active" onClick={onEndBattle}>
            ► CONTINUER
          </button>
        </div>
      ) : (
        <div className="battle-actions">
          {subMenu === null && (
            <div className="action-grid">
              <button
                className="pixel-btn active"
                onClick={() => setSubMenu("attack")}
                disabled={turn !== "player"}
              >
                ⚔️ ATTAQUE
              </button>
              <button
                className="pixel-btn active"
                onClick={() => setSubMenu("balls")}
                disabled={turn !== "player"}
              >
                🔴 BALL
              </button>
              <button
                className="pixel-btn active"
                onClick={() => setSubMenu("bag")}
                disabled={turn !== "player"}
              >
                🎒 SAC
              </button>
              <button
                className="pixel-btn red"
                onClick={onFlee}
                disabled={turn !== "player"}
              >
                🏃 FUITE
              </button>
            </div>
          )}

          {subMenu === "attack" && (
            <div className="submenu pixel-box">
              <div className="submenu-title">CHOISIR UNE ATTAQUE :</div>
              <div className="move-grid">
                {wildPokemon?.moves.map((move) => (
                  <button
                    key={move.name}
                    className="move-btn pixel-box"
                    onClick={() => { onAttack(move); setSubMenu(null); }}
                  >
                    <div className="move-name">{move.name}</div>
                    <div className="move-power">{move.power > 0 ? `PUI ${move.power}` : "STATUT"}</div>
                  </button>
                ))}
              </div>
              <button className="pixel-btn-sm" onClick={() => setSubMenu(null)}>◄ RETOUR</button>
            </div>
          )}

          {subMenu === "balls" && (
            <div className="submenu pixel-box">
              <div className="submenu-title">LANCER UNE BALL :</div>
              {balls.length === 0 ? (
                <div className="empty-msg">Aucune Ball dans le sac !</div>
              ) : (
                <div className="item-list">
                  {balls.map((ball) => (
                    <button
                      key={ball.id}
                      className="item-use-btn pixel-box"
                      onClick={() => { onCapture(ball.id); setSubMenu(null); }}
                    >
                      <span>{ball.icon}</span>
                      <span>{ball.name}</span>
                      <span className="item-qty">×{bag[ball.id]}</span>
                    </button>
                  ))}
                </div>
              )}
              <button className="pixel-btn-sm" onClick={() => setSubMenu(null)}>◄ RETOUR</button>
            </div>
          )}

          {subMenu === "bag" && (
            <div className="submenu pixel-box">
              <div className="submenu-title">UTILISER UN OBJET :</div>
              {potions.length === 0 ? (
                <div className="empty-msg">Aucune Potion dans le sac !</div>
              ) : (
                <div className="item-list">
                  {potions.map((item) => (
                    <button
                      key={item.id}
                      className="item-use-btn pixel-box"
                      onClick={() => { onPotion(item.id); setSubMenu(null); }}
                    >
                      <span>{item.icon}</span>
                      <span>{item.name}</span>
                      <span className="item-qty">×{bag[item.id]}</span>
                    </button>
                  ))}
                </div>
              )}
              <button className="pixel-btn-sm" onClick={() => setSubMenu(null)}>◄ RETOUR</button>
            </div>
          )}
        </div>
      )}
    </div>
  );
};

export default BattleScreen;
