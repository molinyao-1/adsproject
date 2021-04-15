#include "lib_pidctrl.h"
/*!
 * @addtogroup pidctrl
 * @{
 */

/**
 * @brief ����PID�ṹ�����
 *
 * @param _pid  Ҫ���õ�PID�����ṹ��ָ��
 * @param _kp  ����ϵ��
 * @param _ki  ����ϵ��
 * @param _kd  ΢��ϵ��
 */
void PIDCTRL_Setup(pidCtrl_t *_pid, float _kp, float _ki, float _kd)
{
    _pid->kp = _kp;
    _pid->ki = _ki;
    _pid->kd = _kd;
    _pid->errCurr = _pid->errIntg = _pid->errIntg = _pid->errPrev = 0.0f;
}

/**
 * @brief ���첢����һ��PID�ṹ��ָ��
 *
 * @param _kp  ����ϵ��
 * @param _ki  ����ϵ��
 * @param _kd  ΢��ϵ��
 * @return pidCtrl_t*  ������ɵ�PID�����ṹ��ָ��
 */
pidCtrl_t* PIDCTRL_Construct(float _kp, float _ki, float _kd)
{
    pidCtrl_t *inst = (pidCtrl_t*) malloc(sizeof(pidCtrl_t));
    assert(inst);
    if (inst == NULL)
    {
        return NULL ;
    }
    PIDCTRL_Setup(inst, _kp, _ki, _kd);
    return inst;
}

/**
 * @brief ����PIDƫ���֪ƫ����
 *
 * @param _pid  PID�����ṹ��ָ��
 * @param _err  ƫ��
 */
void PIDCTRL_ErrUpdate(pidCtrl_t *_pid, float _err)
{
    _pid->errPrev = _pid->errCurr;
    _pid->errCurr = _err;
    _pid->errIntg += _pid->errCurr;
    _pid->errDiff = _pid->errCurr - _pid->errPrev;
}

/**
 * @brief ����PIDƫ���֪΢����
 *
 * @param _pid  PID�����ṹ��ָ��
 * @param _diff  ƫ���΢����
 */
void PIDCTRL_ErrUpdateByDiff(pidCtrl_t *_pid, float _diff)
{
    _pid->errPrev = _pid->errCurr;
    _pid->errCurr = _pid->errPrev + _diff;
    _pid->errIntg += _pid->errCurr;
    _pid->errDiff = _diff;
}

/**
 * @brief ����PIDƫ���֪ƫ�����΢����
 *
 * @param _pid  PID�����ṹ��ָ��
 * @param _err  ƫ��
 * @param _diff  ƫ���΢����
 */
void PIDCTRL_ErrUpdateByErrAndDiff(pidCtrl_t *_pid, float _err, float _diff)
{
    _pid->errPrev = _pid->errCurr;
    _pid->errCurr = _err;
    _pid->errIntg += _pid->errCurr;
    _pid->errDiff = _diff;
}

/**
 * @brief ����������
 *
 * @param _pid  PID�����ṹ��ָ��
 * @return float ����������
 */
float PIDCTRL_CalcPGain(pidCtrl_t *_pid)
{
    return _pid->errCurr * _pid->kp;
}

/**
 * @brief ����������
 *
 * @param _pid  PID�����ṹ��ָ��
 * @return float ���ֽ�����
 */
float PIDCTRL_CalcIGain(pidCtrl_t *_pid)
{
    return _pid->errIntg * _pid->ki;
}

/**
 * @brief ����΢�����
 *
 * @param _pid  PID�����ṹ��ָ��
 * @return float ΢�ֽ�����
 */
float PIDCTRL_CalcDGain(pidCtrl_t *_pid)
{
    return _pid->errDiff * _pid->kd;
}

/**
 * @brief ����PID�����
 *
 * @param _pid  PID�����ṹ��ָ��
 * @return float PID������
 */
float PIDCTRL_CalcPIDGain(pidCtrl_t *_pid)
{
    return PIDCTRL_CalcPGain(_pid) + PIDCTRL_CalcIGain(_pid) + PIDCTRL_CalcDGain(_pid);
}

/**
 * @brief ����ƫ�������PID�������֪ƫ����
 *
 * @param _pid  PID�����ṹ��ָ��
 * @param _err  ƫ��
 * @return float PID������
 */
float PIDCTRL_UpdateAndCalcPID(pidCtrl_t *_pid, float _err)
{
    PIDCTRL_ErrUpdate(_pid, _err);
    return PIDCTRL_CalcPIDGain(_pid);
}


/**
 * @brief ����ƫ�������PID�������֪΢����
 *
 * @param _pid  PID�����ṹ��ָ��
 * @param _diff  ƫ���΢����
 * @return float PID������
 */
float PIDCTRL_UpdateByDiffAndCalcPID(pidCtrl_t *_pid, float _diff)
{
    PIDCTRL_ErrUpdateByDiff(_pid, _diff);
    return PIDCTRL_CalcPIDGain(_pid);
}

/**
 * @brief ���ô���PID�ṹ�������
 *
 * @param _pid  Ҫ���õĴ���PID�����ṹ��ָ��
 * @param _okp  �⻷����ϵ��
 * @param _oki  �⻷����ϵ��
 * @param _okd  �⻷΢��ϵ��
 * @param _ikp  �ڻ�����ϵ��
 * @param _iki  �ڻ�����ϵ��
 * @param _ikd  �ڻ�΢��ϵ��
 */
void PID2CTRL_Setup(pid2Ctrl_t *_pid, float _okp, float _oki, float _okd, float _ikp, float _iki, float _ikd)
{
    PIDCTRL_Setup(&_pid->outer, _okp, _oki, _okd);
    PIDCTRL_Setup(&_pid->inner, _ikp, _iki, _ikd);
}

/**
 * @brief ���첢����һ������PID�ṹ��ָ��
 *
 * @param _okp  �⻷����ϵ��
 * @param _oki  �⻷����ϵ��
 * @param _okd  �⻷΢��ϵ��
 * @param _ikp  �ڻ�����ϵ��
 * @param _iki  �ڻ�����ϵ��
 * @param _ikd  �ڻ�΢��ϵ��
 * @return pid2Ctrl_t*  ����PID�����ṹ��ָ��
 */
pid2Ctrl_t* PID2CTRL_Construct(float _okp, float _oki, float _okd, float _ikp, float _iki, float _ikd)
{
    pid2Ctrl_t *inst = (pid2Ctrl_t*) malloc(sizeof(pid2Ctrl_t));
    assert(inst);
    if (inst == NULL)
    {
        return NULL ;
    }
    PID2CTRL_Setup(inst, _okp, _oki, _okd, _ikp, _iki, _ikd);
    return inst;
}

/**
 * @brief ����ƫ������㴮��PID�������֪ƫ����
 *
 * @param _pid  ����PID�����ṹ��ָ��
 * @param _err  ƫ������
 * @return float ����PID������
 */
float PID2CTRL_UpdateAndCalcPID(pid2Ctrl_t *_pid, float _err)
{
    return PIDCTRL_UpdateAndCalcPID(&_pid->inner, PIDCTRL_UpdateAndCalcPID(&_pid->outer, _err));
}

/**
 * @brief ����ƫ������㴮��PID�������֪΢����
 *
 * @param _pid  ����PID�����ṹ��ָ��
 * @param _diff  ƫ��΢������
 * @return float ����PID������
 */
float PID2CTRL_UpdateByDiffeAndCalcPID(pid2Ctrl_t *_pid, float _diff)
{
    return PIDCTRL_UpdateAndCalcPID(&_pid->inner, PIDCTRL_UpdateByDiffAndCalcPID(&_pid->outer, _diff));
}

/* @} */

