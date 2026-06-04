import React, { useState } from "react";
import { ITEMS, TYPE_COLORS } from "../data/pokemon";

export const BagPanel = ({ bag }) => {
  const bagItems = ITEMS.filter((i) => bag[i.id] > 0);

  return (
    <div className="bag-panel">
      <div className="panel-header pixel-box">
        <span className="pixel-title">🎒 SAC À DOS</span>
        <span className="item-count">{bagItems.reduce((a, i) => a + bag[i.id], 0)} objets</span>
      </div>
      {bagItems.length === 0 ? (
        <div className="empty-bag pixel-box">
          <div>Ton sac est vide !</div>
          <div className="empty-hint">Complète des missions quotidiennes pour gagner des objets.</div>
        </div>
      ) : (
        <div className="bag-grid">
          {bagItems.map((item) => (
            <div key={item.id} className="bag-item pixel-box">
              <div className="bag-item-icon">{item.icon}</div>
              <div className="bag-item-info">
                <div className="bag-item-name">{item.name}</div>
                <div className="bag-item-desc">{item.description}</div>
                <div className="bag-item-rarity" data-rarity={item.rarity}>{item.rarity}</div>
              </div>
              <div className="bag-item-qty">×{bag[item.id]}</div>
            </div>
          ))}
        </div>
      )}
    </div>
  );
};

export const PokedexPanel = ({ caught }) => {
  const [selected, setSelected] = useState(null);

  return (
    <div className="pokedex-panel">
      <div className="panel-header pixel-box">
        <span className="pixel-title">📖 POKÉDEX</span>
        <span className="item-count">{caught.length} capturé{caught.length > 1 ? "s" : ""}</span>
      </div>
      {caught.length === 0 ? (
        <div className="empty-bag pixel-box">
          <div>Aucun Pokémon capturé !</div>
          <div className="empty-hint">Lance des Balls en combat pour attraper des Pokémon.</div>
        </div>
      ) : (
        <div className="pokedex-grid">
          {caught.map((poke) => (
            <button
              key={poke.id}
              className={`pokedex-entry pixel-box ${selected?.id === poke.id ? "selected" : ""}`}
              onClick={() => setSelected(selected?.id === poke.id ? null : poke)}
            >
              <img
                src={`https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/${poke.id}.png`}
                alt={poke.name}
                className="dex-sprite"
                style={{ imageRendering: "pixelated" }}
              />
              <div className="dex-name">{poke.name}</div>
              <div
                className="dex-type"
                style={{ background: TYPE_COLORS[poke.type] || "#888" }}
              >
                {poke.type}
              </div>
            </button>
          ))}
        </div>
      )}

      {selected && (
        <div className="dex-detail pixel-box">
          <div className="dex-detail-header">
            <img
              src={`https://raw.githubusercontent.com/PokeAPI/sprites/master/sprites/pokemon/${selected.id}.png`}
              alt={selected.name}
              className="dex-detail-sprite"
              style={{ imageRendering: "pixelated" }}
            />
            <div>
              <div className="dex-detail-name">{selected.name}</div>
              <div
                className="dex-type"
                style={{ background: TYPE_COLORS[selected.type] || "#888" }}
              >
                {selected.type}
              </div>
              <div className="dex-caught-date">Capturé le {selected.caughtAt}</div>
            </div>
          </div>
          <div className="dex-stats">
            <div className="dex-stat"><span>PV</span><span>{selected.hp}</span></div>
            <div className="dex-stat"><span>ATK</span><span>{selected.atk}</span></div>
            <div className="dex-stat"><span>Rareté</span><span>{selected.rarity}</span></div>
          </div>
        </div>
      )}
    </div>
  );
};
