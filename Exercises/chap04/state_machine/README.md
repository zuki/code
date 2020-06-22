# 4.1 ステートマシン

完成したゲームがセグメンテーションフォールトで動かない。問題は`AIComponent::ChangeState()`の以下の部分だった。

```c++
void AIComponent::ChangeState(const std::string& name)
{
    // 現在の状態を抜け出す
    if (mCurrentState)
    {
        mCurrentState->OnExit();
    }
```

コンパイラオプションのためと思われるが、何も設定されていない`mCurrentState`が
この`if`文で`true`となり、実際は`nullptr`(?)であるので`OnExit`がないためエラーになって
いるようであった。`AIComponent`の作成時に明示的に`nullptr`を設定し、`if`でチェックするように
した。

```c++
AIComponent::AIComponent(class Actor* owner)
:Component(owner)
,mCurrentState(nullptr)
{ }

void AIComponent::ChangeState(const std::string& name)
{
    // 現在の状態を抜け出す
    if (mCurrentState != nullptr)
    {
        mCurrentState->OnExit();
    }
```

### ゲーム結果

```
2020-06-19 15:10:57.331 a.out[23290:118809] INFO: AIPatrol: entered.
2020-06-19 15:10:57.714 a.out[23290:118809] INFO: AIPatrol: exited.
2020-06-19 15:10:57.714 a.out[23290:118809] INFO: AIAttack: entered.
2020-06-19 15:10:57.730 a.out[23290:118809] INFO: AIAttack: nothig occured.
2020-06-19 15:10:57.746 a.out[23290:118809] INFO: AIAttack: exited.
2020-06-19 15:10:57.746 a.out[23290:118809] INFO: AIDeath: entered.
2020-06-19 15:10:57.762 a.out[23290:118809] INFO: AIDeath: survived as a zombie.
2020-06-19 15:10:57.778 a.out[23290:118809] INFO: AIDeath: survived as a zombie.
2020-06-19 15:10:57.794 a.out[23290:118809] INFO: AIDeath: survived as a zombie.
2020-06-19 15:10:57.810 a.out[23290:118809] INFO: AIDeath: survived as a zombie.
2020-06-19 15:10:57.826 a.out[23290:118809] INFO: AIDeath: really dead.
2020-06-19 15:10:57.826 a.out[23290:118809] INFO: AIDeath: exited.
2020-06-19 15:10:57.826 a.out[23290:118809] INFO: Game Over!
```
