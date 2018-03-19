/*
 * Copyright (c) Institute for System Programming of the Russian Academy of Sciences
 * All Rights Reserved
 *
 * Institute for System Programming of the Russian Academy of Sciences (ISP RAS)
 * 25 Alexander Solzhenitsyn st., Moscow, 109004, Russia
 * http://www.ispras.ru
 *
 * The present file was automatically generated on the basis of formal specifications.
 * It is distributed under the same terms and conditions as the derived specifications.
 *
 * N.B. PLEASE DO NOT MODIFY THIS FILE.
 */

package ru.ispras.microtesk.model.x86.op;

import java.util.Map;
import java.math.BigInteger;
import ru.ispras.microtesk.model.Execution;
import ru.ispras.microtesk.model.ProcessingElement;
import ru.ispras.microtesk.model.data.*;
import ru.ispras.microtesk.model.memory.*;
import ru.ispras.microtesk.model.*;
import ru.ispras.microtesk.model.x86.PE;
import ru.ispras.microtesk.model.x86.TempVars;
import ru.ispras.microtesk.model.x86.mode.*;

import static ru.ispras.microtesk.model.x86.TypeDefs.*;

public final class or_m16i16 extends IsaPrimitive {
  private static final class Info extends IsaPrimitiveInfoAnd {
    Info() {
      super(
          IsaPrimitiveKind.OP,
          "or_m16i16",
          or_m16i16.class,
          null
          );
      addArgument("seg_ovrd", RSEG16.INFO);
      addArgument("op1", MOFFS.INFO);
      addArgument("op2", IMM16.INFO);
      addShortcut(new Info_instruction(), "#root");
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final RSEG16 seg_ovrd = (RSEG16) getArgument("seg_ovrd", args);
      final IsaPrimitive op1 = (IsaPrimitive) getArgument("op1", args);
      final IMM16 op2 = (IMM16) getArgument("op2", args);

      return new or_m16i16(seg_ovrd, op1, op2);
    }
  }
  private static final class Info_instruction extends IsaPrimitiveInfoAnd {
    Info_instruction() {
      super(
          IsaPrimitiveKind.OP,
          "or_m16i16",
          instruction.class,
          null
          );
      addArgument("seg_ovrd", RSEG16.INFO);
      addArgument("op1", MOFFS.INFO);
      addArgument("op2", IMM16.INFO);
    }

    @Override
    public IsaPrimitive create(final Map<String, IsaPrimitive> args) {
      final RSEG16 seg_ovrd = (RSEG16) getArgument("seg_ovrd", args);
      final IsaPrimitive op1 = (IsaPrimitive) getArgument("op1", args);
      final IMM16 op2 = (IMM16) getArgument("op2", args);

      return new instruction(
        new or_m16i16(
          seg_ovrd,
          op1,
          op2
        )
      );
    }
  }
  public static final IsaPrimitiveInfoAnd INFO = new Info();

  public final RSEG16 seg_ovrd;
  public final IsaPrimitive op1;
  public final IMM16 op2;

  public or_m16i16(final RSEG16 seg_ovrd, final IsaPrimitive op1, final IMM16 op2) {
    assert RSEG16.INFO.isSupported(seg_ovrd);
    assert MOFFS.INFO.isSupported(op1);
    assert IMM16.INFO.isSupported(op2);

    this.seg_ovrd = seg_ovrd;
    this.op1 = op1;
    this.op2 = op2;

    addArgument("seg_ovrd", seg_ovrd);
    addArgument("op1", op1);
    addArgument("op2", op2);
  }

  @Override
  public void init(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    vars__.mod.access().store(Data.valueOf(Type.CARD(2), 0x0));
    vars__.rm.access().store(Data.valueOf(Type.CARD(3), 0x0));
    op1.init(vars__);
  }

  @Override
  public String syntax(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("or %s:%s, %s", seg_ovrd.text(vars__), op1.text(vars__), op2.text(vars__));
  }

  @Override
  public String image(final TemporaryVariables tempVars) {
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    return String.format("001%s11010000001%s001%s%s%s", seg_ovrd.image(vars__), vars__.mod.access().load().toString(), vars__.rm.access().load().toString(), op1.image(vars__), op2.image(vars__));
  }

  @Override
  public void action(final ProcessingElement procElem, final TemporaryVariables tempVars) {
    final PE pe__ = (PE) procElem;
    final TempVars vars__ = (TempVars) tempVars;
    init(vars__);
    vars__.base.access().store(Data.zeroExtend(addR, seg_ovrd.access(pe__, vars__).load()).shiftLeft(Data.valueOf(addR, 0x4)));
    new or_proc_word(new Immediate(Location.concat(pe__.MEM.access(vars__.base.access().load().add(op1.access(pe__, vars__).load()).add(Data.valueOf(addR, 0x1))), pe__.MEM.access(vars__.base.access().load().add(op1.access(pe__, vars__).load())))), new Immediate(op2.access(pe__, vars__).load().add(Data.valueOf(WORD, 0x0)))).execute(pe__, vars__);
    pe__.MEM.access(vars__.base.access().load().add(op1.access(pe__, vars__).load())).store(vars__.tmpw_res.access().bitField(7, 0).load());
    pe__.MEM.access(vars__.base.access().load().add(op1.access(pe__, vars__).load()).add(Data.valueOf(addR, 0x1))).store(vars__.tmpw_res.access().bitField(15, 8).load());
    op1.execute(pe__, vars__);
    vars__.instruction_image_size.access().store(Data.valueOf(SIZE, 0x5));
  }
}