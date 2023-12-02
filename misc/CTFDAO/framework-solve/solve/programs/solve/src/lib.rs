use anchor_lang::prelude::*;

declare_id!("28prS7e14Fsm97GE5ws2YpjxseFNkiA33tB5D3hLZv3t");

#[program]
pub mod solve {
    use super::*;

    pub fn initialize(ctx: Context<Initialize>) -> Result<()> {
        // solve goes here:
        emit!(chall::ProposalFinalized {
            dao: *ctx.accounts.dao.to_account_info().key,
            id: 0,
            proposer: *ctx.accounts.admin.to_account_info().key,
            quorum_votes: 0,
            for_votes: 0,
            against_votes: 0,
            did_pass: true,
        });
        Ok(())
    }
}

#[derive(Accounts)]
pub struct Initialize<'info> {
    // feel free to expand/change this as needed
    // if you change this, make sure to change framework-solve/src/main.rs accordingly

    #[account(mut)]
    pub admin: AccountInfo<'info>,

    #[account(mut)]
    pub user: Signer<'info>,

    #[account(mut)]
    pub dao: AccountInfo<'info>,

    #[account(mut)]
    pub vault: AccountInfo<'info>,

    #[account(mut)]
    pub proposal: AccountInfo<'info>,

    pub chall: Program<'info, chall::program::Chall>,

    pub system_program: Program<'info, System>,
}