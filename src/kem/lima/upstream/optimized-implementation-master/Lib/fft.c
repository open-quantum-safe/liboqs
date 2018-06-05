
#include "fft.h"

/*
 * Swaps elements in an array to be at the index represented by the number
 * obtained when the bits in the current index are reversed. Elements are
 * swapped in place. Length must be a power of two
*/
void reverse(int x[], int length) 
{
    int j = 0;
    for (int i = 0; i < length - 1; i++) 
    { int k = length >> 1;
      if (i < j) 
        { int tmp = x[i]; x[i] = x[j]; x[j] = tmp; }
      while (k <= j) 
        { j -= k; k = k >> 1; }
      j += k;
    }
}


int log_2(int n)
{
  int i=0;
  while (n!=1) { i++; n>>=1; }
  return i;
}


/* In place variant of fft_sub_1 
 *  - Due to the sizes of q and R=2^32 we can do delayed reduction
 *    in this FFT routine, and do the final reduction at the end
 */
void fft_sub_1(modp_context_t* ctx,int x[], int N, int tau)
{
  reverse(x, N);
  int logLength = log_2(N);
  int taus[32];
  taus[logLength] = tau;
  for (int s = logLength - 1; s >= 0; s--) 
    { taus[s] = mul_mod_q(ctx,taus[s + 1], taus[s + 1]); }
  int m = 1;
    for (int s = 1; s <= logLength; s++) 
      { int h = m;
        m <<=1;
        tau = taus[s];
        int beta = taus[s - 1];
        int omegas[LIMA_MAX_N*2+1];
        omegas[0] = 0;
        for (int k = 0; k < N; k += m) 
          { int omega = tau;
            for (int j = 0; j < h; j++)
              { int v = x[k + j + h];
                int s = mul_mod_q_tag(ctx,omega, v);
                int t = x[k + j];
                x[k + j] = t+s;
                x[k + j + h] = t-s;
	              if (omegas[j]==0)
                    { omega = omegas[j] = mul_mod_q(ctx,omega, beta);
                      omegas[j+1] = 0;
                    }
                else omega = omegas[j];
              }
          }
      }
 
  for (int k = 0; k < N; k++)
    { x[k] %= ctx->q;
      x[k] += (x[k]<0)*ctx->q;
    }
}

/* In place variant of fft_sub_2 
 *  - Due to the sizes of q and R=2^32 we can do delayed reduction
 *    in this FFT routine, and do the final reduction at the end
 */
void fft_sub_2(modp_context_t* ctx, int x[], int N, int tau)
{
  reverse(x, N);
  int logLength = log_2(N);
  int taus[32];
  taus[logLength] = tau;
  for (int s = logLength - 1; s >= 0; s--) 
    { taus[s] = mul_mod_q(ctx,taus[s + 1], taus[s + 1]); }
  int m = 1;
    for (int s = 1; s <= logLength; s++) 
      { int h = m;
        m <<=1;
        tau = taus[s];
        int omegas[LIMA_MAX_N*2+1];
        omegas[0] = 0;
        for (int k = 0; k < N; k += m)
          { int omega = assign_one(ctx);
            for (int j = 0; j < h; j++)
               { int v = x[k + j + h];
                 int s = mul_mod_q_tag(ctx, omega, v);
                 int t = x[k + j];
                 x[k + j] = t + s;
                 x[k + j + h] = t - s;
                 if (omegas[j]==0) 
                    { omega = omegas[j] = mul_mod_q(ctx, omega, tau);
                      omegas[j+1] = 0;
                    }
                 else omega = omegas[j];
               }
          }
      }
    for (int k = 0; k < N; k++)
    { x[k] %= ctx->q;
      x[k] += (x[k]<0)*ctx->q;
    }
}

/* Input and output into different arrays, this avoids a copy 
 * in the calling routine
*/
void bfft(lima_params_t* params, const int a[], int r, int b[])
{
    int N = (int)params->N;
    int p = N + 1;
    int e = params->e;
    int two_pow_e = 1 << e;
    int x[LIMA_MAX_BD];
    int i,t;
    
    if (r==0)
      { 
        for (i = 0; i<p-1; i++) x[i] = mul_mod_q(params->ctx, params->fft_data->powers[r][i], a[i]);
        x[p-1]=0; 
      }
    else
      {
        x[0] = 0;
        for (i = 0; i<p - 1; i++) x[i + 1] = mul_mod_q(params->ctx, params->fft_data->powers[r][i+1], a[i]);
      }
    for (i = p; i<two_pow_e; i++) x[i] = 0;

    fft_sub_2(params->ctx, x, two_pow_e, params->fft_data->beta0);

    for (i = 0; i<two_pow_e; i++) x[i] = mul_mod_q(params->ctx,x[i], params->fft_data->bd[r][i]);

    fft_sub_2(params->ctx,x, two_pow_e, params->fft_data->beta1);
    
    if (r==0)
      { for (i = 0; i<p-1; i++) b[i] = mul_mod_q(params->ctx, x[i + p], params->fft_data->powersi[r][i+1]); }
    else
      { t= mul_mod_q(params->ctx, x[2*p-2], params->fft_data->powersi[r][p-1]);
        for (i = 0; i<p - 1; i++)
          { b[i] = mul_mod_q(params->ctx, x[i + p - 1], params->fft_data->powersi[r][i]);
            b[i] = sub_mod_q(params->ctx, b[i], t);
          }
      }
}

void fft(lima_params_t* params, const int f[], int x[])
{
    int i;
    int N = (int)params->N;

    if (params->mode == LIMA_2P)
    {
        for (i = 0; i<N; i++) x[i] = f[i];
        fft_sub_1(params->ctx, x, N, params->fft_data->alpha0);
    }
    else // LIMA_SP
    {
        bfft(params, f, 0,x);
    }
}

void fft_inv(lima_params_t* params, const int x[], int f[])
{
    int i;
    int N = (int)params->N;
    if (params->mode == LIMA_2P)
    {
        int gamma = mul_mod_q(params->ctx,params->fft_data->alpha1, params->fft_data->alpha1);
        int delta = params->fft_data->beta0;

        for (i = 0; i<N; i++) f[i] = x[i];
        fft_sub_2(params->ctx,f, N, gamma);

        for (i = 0; i<N; i++)
        {
            f[i] = mul_mod_q(params->ctx,f[i], delta);
            delta = mul_mod_q(params->ctx, delta, params->fft_data->alpha1);
        }
    }
    else // LIMA_SP
    {
        bfft(params, x, 1, f);
    }
}


// Naive inverse mod q on standard representation integers
int inv_mod_q(int q, int x)
{
    int q0 = q;
    int t, d;
    int x0 = 0, x1 = 1;

    while (x > 1)
    {
        d = x / q;
        t = q;
        q = x % q;
        x = t;
        t = x0;
        x0 = x1 - d * x0;
        x1 = t;
    }
    if (x1 < 0) x1 += q0;
    return x1; 
}


void precompute_fft_data(lima_params_t* params)
{
    // Convert human readable data into internal representation if needed
    params->fft_data->alpha0 = to_mod_q(params->ctx, params->alpha0);
    params->fft_data->alpha1 = to_mod_q(params->ctx, params->alpha1);
    params->fft_data->beta0  = to_mod_q(params->ctx, params->beta0);
    params->fft_data->beta1  = to_mod_q(params->ctx, params->beta1);

    // Set up Bluestein data
    if (params->mode == LIMA_SP)  
    { 
        int e = params->e;
        int p = (int)params->N + 1;
        int two_pow_e = 1<<e;
        int s, b, r, i;

        int* powers[2] =  { params->fft_data->powers[0],  params->fft_data->powers[1]  };
        int* powersi[2] = { params->fft_data->powersi[0], params->fft_data->powersi[1] };
        int* bd[2] =      { params->fft_data->bd[0],      params->fft_data->bd[1]      };

        powersi[0][0] = inv_mod_q(params->ctx->q, two_pow_e);
        powersi[0][0] = to_mod_q(params->ctx, powersi[0][0]); 
        powersi[1][0] = inv_mod_q(params->ctx->q, (p*two_pow_e)%params->ctx->q);
        powersi[1][0] = to_mod_q(params->ctx, powersi[1][0]);

        for (r = 0; r<2; r++)
        {
            int alpha_r = (r == 0) ? params->fft_data->alpha0 : params->fft_data->alpha1;
            int alpha_1_minus_r = (r == 0) ? params->fft_data->alpha1 : params->fft_data->alpha0;
            
            powers[r][0] = assign_one(params->ctx);
            bd[r][p - 1] = assign_one(params->ctx);
            for (i = 1; i <= p - 1; i++)
            {
                s = (i*i)%(2*p);
                powers[r][i] = exp_mod_q(params->ctx, alpha_r, s);
                powersi[r][i] = mul_mod_q(params->ctx, powers[r][i], powersi[r][0]);
                b = exp_mod_q(params->ctx, alpha_1_minus_r, s);
                bd[r][p - 1 + i] = b;
                bd[r][p - 1 - i] = b;
            }

            for (i = 2 * p - 1; i<two_pow_e; i++) bd[r][i] = 0;
            fft_sub_2(params->ctx, bd[r], two_pow_e, params->fft_data->beta0);
        }
    }
}
