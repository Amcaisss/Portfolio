import { useState, useCallback, useEffect } from "react";
import { WILD_POKEMON, MOVES, ITEMS } from "../data/pokemon";

const PLAYER_MAX_HP = 100;

const getRandomMoves = () => {
  const shuffled = [...MOVES].sort(() => Math.random() - 0.5);
  return shuffled.slice(0, 4);
};

const calcDamage = (atk, power) => {
  if (power === 0) return 0;
  const base = Math.floor((atk * power) / 100) + Math.floor(Math.random() * 10) + 5;
  return base;
};

export const useBattle = ({ bag, useItemInBattle, catchPokemon, recordBattleResult }) => {
  const [phase, setPhase] = useState("idle"); // idle | encounter | battle | catch | result
  const [wildPokemon, setWildPokemon] = useState(null);
  const [wildHp, setWildHp] = useState(0);
  const [playerHp, setPlayerHp] = useState(PLAYER_MAX_HP);
  const [playerMaxHp] = useState(PLAYER_MAX_HP);
  const [log, setLog] = useState([]);
  const [turn, setTurn] = useState("player"); // player | enemy | catch
  const [catchResult, setCatchResult] = useState(null); // null | success | fail
  const [battleOver, setBattleOver] = useState(null); // null | win | lose | caught

  const addLog = (msg) => setLog((prev) => [...prev.slice(-6), msg]);

  const startBattle = useCallback(() => {
    const pool = WILD_POKEMON;
    const pokemon = pool[Math.floor(Math.random() * pool.length)];
    const hp = pokemon.hp + Math.floor(Math.random() * 20);
    setWildPokemon({ ...pokemon, moves: getRandomMoves() });
    setWildHp(hp);
    setPlayerHp(PLAYER_MAX_HP);
    setLog([`Un ${pokemon.name} sauvage apparaît !`]);
    setTurn("player");
    setBattleOver(null);
    setCatchResult(null);
    setPhase("battle");
  }, []);

  const playerAttack = useCallback((move) => {
    if (turn !== "player" || battleOver) return;
    const dmg = calcDamage(50, move.power);
    const newWildHp = Math.max(0, wildHp - dmg);
    setWildHp(newWildHp);

    if (move.power === 0) {
      addLog(`Tu utilises ${move.name}... mais rien ne se passe vraiment.`);
    } else {
      addLog(`Tu utilises ${move.name} ! ${wildPokemon.name} perd ${dmg} PV.`);
    }

    if (newWildHp <= 0) {
      addLog(`${wildPokemon.name} est K.O. ! Victoire !`);
      setBattleOver("win");
      recordBattleResult(true);
      return;
    }
    setTurn("enemy");
  }, [turn, wildHp, wildPokemon, battleOver, recordBattleResult]);

  // Enemy turn
  useEffect(() => {
    if (turn !== "enemy" || battleOver || !wildPokemon) return;
    const timer = setTimeout(() => {
      const move = wildPokemon.moves[Math.floor(Math.random() * wildPokemon.moves.length)];
      const dmg = calcDamage(wildPokemon.atk, move.power);
      const newPlayerHp = Math.max(0, playerHp - dmg);
      setPlayerHp(newPlayerHp);

      if (move.power === 0) {
        addLog(`${wildPokemon.name} utilise ${move.name}...`);
      } else {
        addLog(`${wildPokemon.name} utilise ${move.name} ! Tu perds ${dmg} PV.`);
      }

      if (newPlayerHp <= 0) {
        addLog(`Tu es K.O. ! Tu t'enfuis...`);
        setBattleOver("lose");
        recordBattleResult(false);
        return;
      }
      setTurn("player");
    }, 900);
    return () => clearTimeout(timer);
  }, [turn, battleOver, wildPokemon, playerHp, recordBattleResult]);

  const tryCapture = useCallback((ballId) => {
    if (turn !== "player" || battleOver) return;
    const ball = ITEMS.find((i) => i.id === ballId);
    if (!ball || !bag[ballId]) {
      addLog("Tu n'as pas cette Ball !");
      return;
    }
    useItemInBattle(ballId);
    addLog(`Tu lances une ${ball.name}...`);
    setTurn("catching");

    // Catch rate based on remaining HP %
    const hpRatio = wildHp / (wildPokemon.hp + 20);
    const rate = ball.catchRate * (1 - hpRatio * 0.5);
    const success = Math.random() < rate * 0.6;

    setTimeout(() => {
      if (success) {
        addLog(`Gotcha ! ${wildPokemon.name} a été capturé !`);
        catchPokemon(wildPokemon, ballId);
        setCatchResult("success");
        setBattleOver("caught");
      } else {
        addLog(`${wildPokemon.name} s'est échappé !`);
        setCatchResult("fail");
        setTurn("enemy");
      }
    }, 1200);
  }, [turn, battleOver, bag, wildHp, wildPokemon, useItemInBattle, catchPokemon]);

  const usePotion = useCallback((itemId) => {
    if (turn !== "player" || battleOver) return;
    const item = ITEMS.find((i) => i.id === itemId);
    if (!item || !bag[itemId] || !item.heal) {
      addLog("Tu ne peux pas utiliser cet objet !");
      return;
    }
    useItemInBattle(itemId);
    const healed = Math.min(item.heal, playerMaxHp - playerHp);
    setPlayerHp((prev) => Math.min(prev + item.heal, playerMaxHp));
    addLog(`Tu utilises ${item.name} et récupères ${healed} PV !`);
    setTurn("enemy");
  }, [turn, battleOver, bag, playerHp, playerMaxHp, useItemInBattle]);

  const flee = useCallback(() => {
    addLog("Tu prends la fuite !");
    setBattleOver("flee");
  }, []);

  const endBattle = useCallback(() => {
    setPhase("idle");
    setWildPokemon(null);
    setBattleOver(null);
    setLog([]);
  }, []);

  return {
    phase, setPhase,
    wildPokemon, wildHp,
    playerHp, playerMaxHp,
    log, turn, battleOver, catchResult,
    startBattle, playerAttack, tryCapture, usePotion, flee, endBattle,
  };
};
